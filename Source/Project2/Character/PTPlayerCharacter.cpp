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
#include "PTComponent/PTFactionComponent.h"
#include "PTComponent/Equipment/PTEquipmentComponent.h"

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
	EquipmentComponent = CreateDefaultSubobject<UPTEquipmentComponent>(TEXT("EquipmentComponent"));
}

void APTPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 리소스에 붙어 있는 총 숨김
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	FactionComponent->SetFaction(EFaction::Ally);
}

void APTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerInputComponent->Init(CameraBoom);
	PlayerInputComponent->SetCharacterControl(ECharacterControlType::Shoulder);

	EquipmentComponent->Init();
}

UPTCharacterStatComponent* APTPlayerCharacter::GetStatComponent()
{
	return StatComponent;
}

// BeginPlay보다 먼저 호출됨
void APTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* Component)
{
	Super::SetupPlayerInputComponent(Component);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(Component);
	PlayerInputComponent->SetupPlayerInputComponent(EnhancedInputComponent);
}

void APTPlayerCharacter::Evation()
{
	//TODO: 회피중엔 재장전 캔슬
	
	Super::Evation();
	
	FVector Direction = (MoveComponent->Velocity.Size() != 0) ?
		MoveComponent->Velocity.GetSafeNormal2D() : GetActorRotation().Vector().GetSafeNormal2D();
	
	MoveComponent->MoveToDirection(Direction, 400.f, 0.3f);	
}

void APTPlayerCharacter::StartAttack()
{
	if (IsReloading)
	{
		return;
	}
	
	if (EquipmentComponent->GetCurrentGun()->PullTrigger() == false)
	{
		Reloading();
		return;
	}
}

void APTPlayerCharacter::StopAttack()
{
	EquipmentComponent->GetCurrentGun()->StopTrigger();

}

void APTPlayerCharacter::ReloadAction()
{
	Reloading();
}

void APTPlayerCharacter::EquipInput(EEquipType EquipType)
{
	if (IsReloading || EquipmentComponent->GetCurrentGun()->GetIsFiring())
	{
		return;
	}
	
	EquipmentComponent->ChangeEquipment(EquipType);
}

void APTPlayerCharacter::Reloading()
{
	AGun* CurrentGun = EquipmentComponent->GetCurrentGun();

	if (!IsReloading && !CurrentGun->GetIsFiring())
	{
		IsReloading = true;

		CurrentGun->OnCompleteReload.AddLambda(
		[&](){
			IsReloading = false;			
		});
		
		//TODO: 재장전가속 스텟 전달, 최종 시간 반환받기
		float ReloadAccelerationRateStat = 0.f;
		float ReloadTime = CurrentGun->Reloading(ReloadAccelerationRateStat);
	
		//TODO: 재장전 애니메이션 재생, GunData.ReloadTime에 접근해서 재장전 애니메이션 속도 조절 필요

	}
}

void APTPlayerCharacter::Dead()
{	
	Super::Dead();

	StopAttack();
	
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
	if (!InHUDWidget)
	{
		return;
	}
	
	InHUDWidget->UpdateStat(StatComponent->GetBaseStat(), StatComponent->GetModifierStat());
	InHUDWidget->UpdateHpBar(StatComponent->GetCurrentHp());
	StatComponent->OnStatChanged.AddUObject(InHUDWidget, &UPTHUDWidget::UpdateStat);
	StatComponent->OnHpChanged.AddUObject(InHUDWidget, &UPTHUDWidget::UpdateHpBar);
	
	EquipmentComponent->OnChangeEquip.BindLambda([this, InHUDWidget](AGun* NewEquipment){
		InHUDWidget->UpdateGunAmmo(NewEquipment->GetCurrentAmmo(), NewEquipment->GetMaxAmmo());

		NewEquipment->OnChangeAmmo.Clear();
		NewEquipment->OnStartReload.Clear();
		NewEquipment->OnCompleteReload.Clear();
		
		NewEquipment->OnChangeAmmo.AddUObject(InHUDWidget, &UPTHUDWidget::UpdateGunAmmo);
		NewEquipment->OnStartReload.AddLambda([this, InHUDWidget](){
			InHUDWidget->UpdateGunReloadImg(true);
		});
		NewEquipment->OnCompleteReload.AddLambda([this, InHUDWidget](){
			InHUDWidget->UpdateGunReloadImg(false);
		});
	});
}
