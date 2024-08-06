#include "Character/PTPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "PTComponent/PTInputComponent.h"
#include "PTComponent/Character/PTPlayerStatComponent.h"
#include "PTComponent/Character//PTCharacterMoveComponent.h"
#include "PTComponent/PTFactionComponent.h"
#include "Physics/PTCollision.h"
#include "PTInterface/PTGameInterface.h"
#include "UI/PTHUDWidget.h"

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

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	FactionComponent->SetFaction(EFaction::Player);
}

void APTPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayerInputComponent->Init(CameraBoom);
	PlayerInputComponent->SetCharacterControl(ECharacterControlType::Shoulder);
}

void APTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipmentComponent->Init();
}

void APTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* Component)
{
	Super::SetupPlayerInputComponent(Component);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(Component);
	PlayerInputComponent->SetupPlayerInputComponent(EnhancedInputComponent);
}

UPTCharacterStatComponent* APTPlayerCharacter::GetStatComponent()
{
	return StatComponent;
}

void APTPlayerCharacter::Dead()
{	
	Super::Dead();
	
	StopAttack();
	
	if (IPTGameInterface* PTGameMode = Cast<IPTGameInterface>(GetWorld()->GetAuthGameMode()))
	{
		PTGameMode->OnPlayerDead(this);
	}
}

void APTPlayerCharacter::Dash()
{
	FVector Direction = (MoveComponent->Velocity.Size() != 0) ?
		MoveComponent->Velocity.GetSafeNormal2D() : GetActorRotation().Vector().GetSafeNormal2D();
	
	MoveComponent->MoveToDirection(Direction, 400.f, 0.3f);
}

void APTPlayerCharacter::StartAttack()
{
	if (bIsReloading)
	{
		return;
	}
	
	if (EquipmentComponent->GetCurrentGun()->GetCurrentAmmo() <= 0)
	{
		Reloading();
		return;
	}

	EquipmentComponent->GetCurrentGun()->PullTrigger();
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
	if (bIsReloading || EquipmentComponent->GetCurrentGun()->GetIsFiring())
	{
		return;
	}
	
	EquipmentComponent->ChangeEquipment(EquipType);
}

void APTPlayerCharacter::Reloading()
{
	APTGun* CurrentGun = EquipmentComponent->GetCurrentGun();

	if (!bIsReloading && !CurrentGun->GetIsFiring())
	{
		bIsReloading = true;

		CurrentGun->OnCompleteReload.AddLambda(
		[&](){
			bIsReloading = false;			
		});
		
		float ReloadAccelerationRateStat = 0.f;

		//TODO: 재장전 애니메이션 재생, GunData.ReloadTime에 접근해서 재장전 애니메이션 속도 조절 필요
		float ReloadTime = CurrentGun->Reloading(ReloadAccelerationRateStat);
	}
}

void APTPlayerCharacter::SetupHUDWidget(UPTHUDWidget* InHUDWidget)
{
	if (!InHUDWidget)
	{
		return;
	}
	
	StatComponent->OnStatChanged.AddUObject(InHUDWidget, &UPTHUDWidget::UpdateStat);
	StatComponent->OnHpChanged.AddUObject(InHUDWidget, &UPTHUDWidget::UpdateHpBar);

	InHUDWidget->UpdateStat(StatComponent->GetBaseStat(), StatComponent->GetModifierStat());
	InHUDWidget->UpdateHpBar(StatComponent->GetCurrentHp());
	InHUDWidget->UpdateEquipWeapon(true);
	
	EquipmentComponent->OnChangeEquip.BindLambda([this, InHUDWidget](EEquipType NewEquipType, APTGun* NewEquipment){
		InHUDWidget->UpdateGunAmmo(NewEquipment->GetCurrentAmmo(), NewEquipment->GetMaxAmmo());
		InHUDWidget->UpdateEquipWeapon(NewEquipType == EEquipType::Main);
		
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
