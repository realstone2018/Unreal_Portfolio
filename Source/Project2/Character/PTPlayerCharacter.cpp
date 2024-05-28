#include "Character/PTPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "PTInputComponent.h"
#include "Character/CharacterStat/PTCharacterStatComponent.h"
#include "GameData/PTGameDataSingleton.h"
#include "GameFramework/CharacterMovementComponent.h"	// for GetCharacterMovement()
#include "Weapon/Gun.h"

APTPlayerCharacter::APTPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	PlayerInputComponent = CreateDefaultSubobject<UPTInputComponent>(TEXT("InputComponet"));

}

// BeginPlay보다 먼저 호출됨
void APTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* Component)
{
	UE_LOG(LogTemp, Display, TEXT("PTPlayerCharacter.SetupPlayerInputComponent()"));

	Super::SetupPlayerInputComponent(Component);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(Component);
	PlayerInputComponent->SetupPlayerInputComponent(EnhancedInputComponent);
}

void APTPlayerCharacter::BeginPlay()
{
	UE_LOG(LogTemp, Display, TEXT("PTPlayerCharacter.BeginPlay()"));
	
	Super::BeginPlay();

	PlayerInputComponent->Init(CameraBoom);
	PlayerInputComponent->SetCharacterControl(ECharacterControlType::Shoulder);

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	Gun->SetGunData(UPTGameDataSingleton::Get().GetGunData("Rifle"));
}

void APTPlayerCharacter::StartAttack()
{
	UE_LOG(LogTemp, Display, TEXT("PTPlayerCharacter.Attack()"));
	
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

void APTPlayerCharacter::Reloading()
{
	if (!bIsReloading)
	{
		bIsReloading = true;

		Gun->OnCompleteReload.BindLambda(
			[&]()
			{
				bIsReloading = false;			
			}
		);
		
		//TODO: 재장전가속 스텟 전달, 최종 시간 반환받기
		float ReloadAccelerationRateStat = 0.f;
		float ReloadTime = Gun->Reloading(ReloadAccelerationRateStat);
	
		//TODO: 재장전 애니메이션 재생, GunData.ReloadTime에 접근해서 재장전 애니메이션 속도 조절 필요



		
	}
}
