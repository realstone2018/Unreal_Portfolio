#include "ShooterCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Weapon/PTGun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

AShooterCharacter::AShooterCharacter()	
{
	PrimaryActorTick.bCanEverTick = true;


#pragma region Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Udemy/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Udemy/Input/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Udemy/Input/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Udemy/Input/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShootRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Udemy/Input/IA_Shoot.IA_Shoot'"));
	if (nullptr != InputActionShootRef.Object)
	{
		ShootAction = InputActionShootRef.Object;
	}
#pragma endregion 
}


void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	SetCharacterControl();

	//AGun을 상속받는 블루프린트(GunClass) 스폰 
	Gun = GetWorld()->SpawnActor<APTGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}


#pragma region Input
void AShooterCharacter::SetCharacterControl()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr)
	{
		return;
	}
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterCharacter::MoveCallback);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterCharacter::LookCallback);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Jump);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AShooterCharacter::ShootCalllback);
}

void AShooterCharacter::MoveCallback(const FInputActionValue& Value)
{
	// Look만 DeltaSeconds를 적용한 이유 - 이동 내부로직에서, 인자로 받은 값에 따라 처리하고 있음 (인자가 이동거리가 아닌 속도의 개념)
	// 멀티에서는 서버에서 이를 조정, 로컬은 회전만 감도설정과 DeltaTime에 따라 조정하면 된다.  
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AShooterCharacter::LookCallback(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	LookAxisVector *= RotationRate * GetWorld()->GetDeltaSeconds();
	
	// 반대로 움직이는 이유 찾지 못함, 일단 - 로 
	AddControllerYawInput(-LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AShooterCharacter::ShootCalllback()
{
	Gun->PullTrigger();
}
#pragma endregion


#pragma region Battle
float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	if (IsDead())
	{
		Dead();
	}
	
	return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

void AShooterCharacter::Dead()
{
	ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
	if (GameMode != nullptr)
	{
		GameMode->PawnKilled(this);
	}
	
	// 더이상 어떤 행동도 하지 않도록 컨트롤러를 폰에서 분리 
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


#pragma endregion 




