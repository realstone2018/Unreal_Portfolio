#include "Character/PTMonster.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "PTGameModeBase.h"
#include "PTComponent/Character/PTCharacterMoveComponent.h"
#include "PTComponent/PTFactionComponent.h"
#include "Physics/PTCollision.h"
#include "AI/Monster/PTMonsterAIController.h"
#include "UI/PTWidgetComponent.h"

#define ENABLE_DRAW_DEBUG 0

APTMonster::APTMonster()
{
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PTMONSTER);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	AIControllerClass = APTMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MonsterStat = CreateDefaultSubobject<UPTMonsterStatComponent>(TEXT("MonsterStat"));

	FactionComponent->SetFaction(EFaction::Monster);
}

void APTMonster::Initialize()
{
	//TODO: 몬스터 Enum을 넘겨서 처리하도록 변경하기
	MonsterStat->SetMonsterStat(FName("Scorch"));
	
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PTMONSTER);
	SetActorEnableCollision(true);
	
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents(SkeletalMeshComponents);
	
	for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
	{
		SkeletalMeshComponent->SetComponentTickEnabled(true);
	}

	if (APTMonsterAIController* AIController = Cast<APTMonsterAIController>(GetInstigatorController()))
	{
		AActor* DefaultTarget = nullptr;
		
		IPTGameInterface* GameMode = Cast<IPTGameInterface>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			DefaultTarget = GameMode->GetMainStation();
		}
		
		AIController->RunAI(DefaultTarget);
	}

	MoveComponent->SetMovementMode(EMovementMode::MOVE_Walking);

	HpBar->SetHiddenInGame(false);
}

void APTMonster::Terminate()
{
	OnDead.Unbind();
	SetActorHiddenInGame(true);

	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	
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

	if (APTMonsterAIController* AIController = Cast<APTMonsterAIController>(GetInstigatorController()))
	{
		AIController->StopAI();
	}
}

UPTCharacterStatComponent* APTMonster::GetStatComponent()
{
	return MonsterStat;
}

void APTMonster::Dead()
{
	Super::Dead();
	
	SetActorTickEnabled(false);
 
	if (APTMonsterAIController* AIController = Cast<APTMonsterAIController>(GetInstigatorController()))
	{
		AIController->StopAI();
	}

	OnDead.ExecuteIfBound(this);
}

void APTMonster::Dash()
{
	Jump();
	
	FVector Direction = (MoveComponent->Velocity.Size() != 0) ?
	MoveComponent->Velocity.GetSafeNormal2D() : GetActorRotation().Vector().GetSafeNormal2D();
	
	MoveComponent->MoveToDirection(Direction, 500.f, 0.5f);
}

void APTMonster::Attack()
{
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	for (AActor* HitTarget : HitTargets)
	{
		Params.AddIgnoredActor(HitTarget);	
	}
	
	const float AttackRadius = MonsterStat->GetAttackRadius();
	const float AttackDamage = MonsterStat->GetAttackDamage();
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRadius;

	TArray<FHitResult> OutHitResult;
	uint8 HitDetected = GetWorld()->SweepMultiByChannel(OutHitResult, Start, End,
		FQuat::Identity, CCHANNEL_PTMONSTER_MELEE, FCollisionShape::MakeSphere(AttackRadius), Params);

	for(FHitResult HitResult : OutHitResult)
	{
		if (APTCharacterBase* HitCharacter = Cast<APTCharacterBase>(HitResult.GetActor()))
		{
			if (HitCharacter->IsDead())
			{
				continue;
			}
		}
		HitTargets.Add(HitResult.GetActor());
		
		FPointDamageEvent PointDamageEvent;
		PointDamageEvent.HitInfo.ImpactPoint = HitResult.ImpactPoint;
		HitResult.GetActor()->TakeDamage(AttackDamage, PointDamageEvent, GetController(), this);		
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugSphere(GetWorld(), CapsuleOrigin, AttackRadius, 10, DrawColor, false, 0.5f);
#endif
}

void APTMonster::PlayAttackMontage()
{
	HitTargets.Empty();
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackMontage, 1.0f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APTMonster::EndAttackMontage);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void APTMonster::OnNotifyAttack()
{
	Super::OnNotifyAttack();

	Attack();
}

void APTMonster::EndAttackMontage(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (OnAttackFinished.IsBound())
	{
		OnAttackFinished.Execute();
	}

	HitTargets.Empty();
}

void APTMonster::AttackByAI(float& AttackCooldown)
{
	PlayAttackMontage();
	AttackCooldown = MonsterStat->GetAttackCooldown();
}

void APTMonster::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}