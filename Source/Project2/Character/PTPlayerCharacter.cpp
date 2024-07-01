#include "Character/PTPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "PTComponent/PTInputComponent.h"
#include "PTComponent/Character/PTPlayerStatComponent.h"
#include "GameData/PTGameDataSingleton.h"
#include "GameFramework/CharacterMovementComponent.h"	// for GetCharacterMovement()
#include "Weapon/Gun.h"
#include "Physics/PTCollision.h"
#include "Components/CapsuleComponent.h"
#include "PTComponent/Character//PTCharacterMoveComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "UI/PTHUDWidget.h"
#include "PTComponent/Character/PTCharacterStatComponent.h"

APTPlayerCharacter::APTPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PTPLAYER);

	PlayerInputComponent = CreateDefaultSubobject<UPTInputComponent>(TEXT("InputComponet"));	
	StatComponent = CreateDefaultSubobject<UPTPlayerStatComponent>(TEXT("PlayerStatComponent"));
}

void APTPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 리소스에 붙어 있는 총 숨김
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	Gun->SetGunData(UPTGameDataSingleton::Get().GetGunData("BaseRifle"));
}

void APTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerInputComponent->Init(CameraBoom);
	PlayerInputComponent->SetCharacterControl(ECharacterControlType::Shoulder);
}

UPTCharacterStatComponent* APTPlayerCharacter::GetStatComponent()
{
	UE_LOG(LogTemp, Display, TEXT("APTPlayerCharacter::GetStatComponent()"));

	return StatComponent;
}

// BeginPlay보다 먼저 호출됨
void APTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* Component)
{
	UE_LOG(LogTemp, Display, TEXT("PTPlayerCharacter.SetupPlayerInputComponent()"));

	Super::SetupPlayerInputComponent(Component);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(Component);
	PlayerInputComponent->SetupPlayerInputComponent(EnhancedInputComponent);
}

void APTPlayerCharacter::Evation()
{
	//TODO: 회피중엔 재장전 캔슬

	
	Super::Evation();

	UE_LOG(LogTemp, Display, TEXT("PTPlayerCharacter::Evation()"));

	FVector Direction = (MoveComponent->Velocity.Size() != 0) ?
		MoveComponent->Velocity.GetSafeNormal2D() : GetActorRotation().Vector().GetSafeNormal2D();
	
	MoveComponent->MoveToDirection(Direction, 400.f, 0.3f);	
}

void APTPlayerCharacter::StartAttack()
{
	UE_LOG(LogTemp, Display, TEXT("PTPlayerCharacter.Attack()"));
	if (bIsReloading)
	{
		return;
	}
	
	if (Gun->PullTrigger() == false)
	{
		Reloading();	
	} 
}

void APTPlayerCharacter::StopAttack()
{
	UE_LOG(LogTemp, Display, TEXT("PTPlayerCharacter.StopAttack()"));

	Gun->StopTrigger();

}

void APTPlayerCharacter::ReloadAction()
{
	Reloading();
}

void APTPlayerCharacter::Reloading()
{
	if (!bIsReloading)
	{
		bIsReloading = true;

		Gun->OnCompleteReload.AddLambda(
		[&](){
			bIsReloading = false;			
		});
		
		//TODO: 재장전가속 스텟 전달, 최종 시간 반환받기
		float ReloadAccelerationRateStat = 0.f;
		float ReloadTime = Gun->Reloading(ReloadAccelerationRateStat);
	
		//TODO: 재장전 애니메이션 재생, GunData.ReloadTime에 접근해서 재장전 애니메이션 속도 조절 필요

	}
}

void APTPlayerCharacter::Dead()
{	
	Super::Dead();
	
	ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
	if (GameMode != nullptr)
	{
		//GameMode->PawnKilled(this);
	}
	
	// 더이상 어떤 행동도 하지 않도록 컨트롤러를 폰에서 분리 
	DetachFromControllerPendingDestroy();
}

void APTPlayerCharacter::SetupHUDWidget(UPTHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(StatComponent->GetBaseStat(), StatComponent->GetModifierStat());
		InHUDWidget->UpdateHpBar(StatComponent->GetCurrentHp());
		StatComponent->OnStatChanged.AddUObject(InHUDWidget, &UPTHUDWidget::UpdateStat);
		StatComponent->OnHpChanged.AddUObject(InHUDWidget, &UPTHUDWidget::UpdateHpBar);

		InHUDWidget->UpdateGunAmmo(Gun->GetCurrentAmmo(), Gun->GetMaxAmmo());
		Gun->OnChangeAmmo.AddUObject(InHUDWidget, &UPTHUDWidget::UpdateGunAmmo);
		Gun->OnStartReload.AddLambda(
[this, InHUDWidget](){
			InHUDWidget->UpdateGunReloadImg(true);
		});
		Gun->OnCompleteReload.AddLambda(
		[this, InHUDWidget](){
			InHUDWidget->UpdateGunReloadImg(false);
		});
	}
}
