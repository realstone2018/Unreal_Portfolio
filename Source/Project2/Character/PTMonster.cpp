// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PTMonster.h"
#include "Components/CapsuleComponent.h"
#include "Physics/PTCollision.h"
#include "Engine/DamageEvents.h"
#include "PTComponent/Character/PTCharacterMoveComponent.h"
#include "AI/Monster/PTScorchAIController.h"
#include "PTComponent/PTFactionComponent.h"
#include "PTComponent/Character/PTMonsterStatComponent.h"

APTMonster::APTMonster()
{
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PTMONSTER);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	AIControllerClass = APTScorchAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;	//배치npc, 런타임중 스폰npc 둘다 AABAIController 적용

	MonsterStat = CreateDefaultSubobject<UPTMonsterStatComponent>(TEXT("MonsterStat"));
}

//TODO: 이거 타이밍이 어떻게 되더라
void APTMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
// 	FPTCharacterStat CharacterStat = UPTGameDataSingleton::Get().GetCharacterStat(CurrentLevel);
// 	SetBaseStat(CharacterStat);
// 	
// 	//TODO: Dirty 패턴 적용하기
// 	SetHp(BaseStat.MaxHp);

	FactionComponent->SetFaction(EFaction::Enemy);
}

void APTMonster::Instantiate()
{
	//TODO: 몬스터 Enum을 넘겨서 처리하도록 변경하기
	MonsterStat->SetMonsterStat(FName("Scorch"));
	
	SetActorHiddenInGame(false);

	SetActorEnableCollision(true);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PTMONSTER);
	
	SetActorTickEnabled(true);
	
	// 애니메이션 활성화
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents(SkeletalMeshComponents);
	
	for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
	{
		SkeletalMeshComponent->SetComponentTickEnabled(true);
	}

	// 비헤이비어 트리 활성화
	if (APTScorchAIController* AIController = Cast<APTScorchAIController>(GetInstigatorController()))
	{
		AIController->RunAI();
	}

	MoveComponent->SetMovementMode(EMovementMode::MOVE_Walking);

	HpBar->SetHiddenInGame(false);
}

void APTMonster::Dispose()
{
	OnDead.Unbind();
	SetActorHiddenInGame(true);

	//TODO: Dead()에 중복 코드들 관리 
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	
	// 애니메이션 비활성화
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents(SkeletalMeshComponents);
	
	for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
	{
		SkeletalMeshComponent->SetComponentTickEnabled(false);
		if (UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance())
		{
			AnimInstance->StopAllMontages(0.0f);
		}
	}

	// 비헤이비어 트리 비활성화
	// TODO: APTScorchAIController를 APMonsterAIController로 변경
	//       APMonsterAIController 하나로 모든 몬스터를 관리 + BT와 BB만 교체해줘서 행동패턴을 다르게 
	if (APTScorchAIController* AIController = Cast<APTScorchAIController>(GetInstigatorController()))
	{
		AIController->StopAI();
	}

}

UPTCharacterStatComponent* APTMonster::GetStatComponent()
{
	return MonsterStat;
}

void APTMonster::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackMontage, 1.0f);

	//몽타주 종료 델리게이트 등록
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APTMonster::EndAttackMontage);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void APTMonster::OnNotifyAttack()
{
	Super::OnNotifyAttack();

	Attack();
}

// Call by Montage Notify
void APTMonster::Attack()
{
	// FString string = MoveComponent->GetIsMoving() ? "Yes" : "No";
	// UE_LOG(LogTemp, Display, TEXT("PTMonster::Attack() - IsMoving: %s"), *string);
	//
	// if (!MoveComponent->GetIsMoving())
	// {
	// 	FVector Direction = (MoveComponent->Velocity.Size() != 0) ?
	// 	MoveComponent->Velocity.GetSafeNormal2D() : GetActorRotation().Vector().GetSafeNormal2D();
	// 	//Direction.Z = 0.3f;
	// 	MoveComponent->MoveToDirection(Direction, 150.f, 0.2f);
	//
	// 	UE_LOG(LogTemp, Display, TEXT("MoveComponent->MoveToDirection"));
	// }


	// FVector Direction = (MoveComponent->Velocity.Size() != 0) ?
	// 	MoveComponent->Velocity.GetSafeNormal2D() : GetActorRotation().Vector().GetSafeNormal2D();
	// MoveComponent->MoveInput(Direction, 10.f, 0.2f);

	Jump();
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	
	const float AttackRange = MonsterStat->GetMonsterStat().AttackRange;
	const float AttackRadius = MonsterStat->GetAttackRadius();

	//const float AttackDamage = StatComponent->GetTotalStat().Attack;
	const float AttackDamage = 1.f;
	
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	FHitResult OutHitResult;
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_PTMONSTER_MELEE, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected) {
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG
	//센터
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	//둥근부분을 뺀 길이의 절반
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 0.5f);
#endif

}

void APTMonster::Dead()
{
	Super::Dead();

	
	//SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	
	// 비헤이비어 트리 비활성화
	// TODO: APTScorchAIController를 APMonsterAIController로 변경
	//       APMonsterAIController 하나로 모든 몬스터를 관리 + BT와 BB만 교체해줘서 행동패턴을 다르게 
	if (APTScorchAIController* AIController = Cast<APTScorchAIController>(GetInstigatorController()))
	{
		AIController->StopAI();
	}

	OnDead.ExecuteIfBound(this);
}

void APTMonster::EndAttackMontage(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UE_LOG(LogTemp, Display, TEXT("PTMonster::EndAttackMontage()"));
	
}
