#include "PTInputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/PTCharacterControlData.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "PTInterface/PTPlayerInputInterface.h"
#include "GameFramework/CharacterMovementComponent.h"	// for GetCharacterMovement()

UPTInputComponent::UPTInputComponent()
{
	CurrentCharacterControlType = ECharacterControlType::Shoulder;

	static ConstructorHelpers::FObjectFinder<UPTCharacterControlData> ShoulderDataRef(TEXT("/Script/Project2.PTCharacterControlData'/Game/Project2/Input/ControlData/PC_Shoulder.PC_Shoulder'"));
	if (ShoulderDataRef.Object) {	
		CharacterControlMap.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UPTCharacterControlData> QuaterDataRef(TEXT("/Script/Project2.PTCharacterControlData'/Game/Project2/Input/ControlData/PC_Quater.PC_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlMap.Add(ECharacterControlType::Quater, QuaterDataRef.Object);	
	}

	static ConstructorHelpers::FObjectFinder<UPTCharacterControlData> ZoomDataRef(TEXT("/Script/Project2.PTCharacterControlData'/Game/Project2/Input/ControlData/PC_Zoom.PC_Zoom'"));
	if (ZoomDataRef.Object)
	{
		CharacterControlMap.Add(ECharacterControlType::Zoom, ZoomDataRef.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project2/Input/Action/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object) {
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}
     
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project2/Input/Action/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object) {
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}
     
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project2/Input/Action/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object) {
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}
		
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDashRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project2/Input/Action/IA_Dash.IA_Dash'"));
	if (nullptr != InputActionDashRef.Object)
	{
		DashAction = InputActionDashRef.Object;
	}
     
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShootRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project2/Input/Action/IA_Shoot.IA_Shoot'"));
	if (nullptr != InputActionShootRef.Object)
	{
		ShootAction = InputActionShootRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionReloadRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project2/Input/Action/IA_Reload.IA_Reload'"));
	if (nullptr != InputActionReloadRef.Object)
	{
		ReloadAction = InputActionReloadRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionZoomRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project2/Input/Action/IA_Zoom.IA_Zoom'"));
	if (nullptr != InputActionZoomRef.Object)
	{
		ZoomAction = InputActionZoomRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EuqipMainActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project2/Input/Action/IA_EuqipMain.IA_EuqipMain'"));
	if (nullptr != EuqipMainActionRef.Object)
	{
		EuqipMainAction = EuqipMainActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EuqipSubActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project2/Input/Action/IA_EuqipSub.IA_EuqipSub'"));
	if (nullptr != EuqipSubActionRef.Object)
	{
		EuqipSubAction = EuqipSubActionRef.Object;
	}
}

void UPTInputComponent::Init(USpringArmComponent* SpringArm)
{
	Character = Cast<ACharacter>(GetOwner());
	MovementComponent = Character->GetCharacterMovement();
	SpringArmComponent = SpringArm;
}

void UPTInputComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &UPTInputComponent::OnDashInput);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &UPTInputComponent::OnStartAttackInput);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &UPTInputComponent::OnCompleteAttackInout);
	
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &UPTInputComponent::OnShoulderMoveInput);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &UPTInputComponent::OnShoulderLookInput);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &UPTInputComponent::OnQuaterMoveInput);

	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UPTInputComponent::OnReloadInput);

	EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &UPTInputComponent::OnZoomInInput);
	EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &UPTInputComponent::OnZoomOutInput);

	EnhancedInputComponent->BindAction(EuqipMainAction, ETriggerEvent::Triggered, this, &UPTInputComponent::OnEquipMainInput);
	EnhancedInputComponent->BindAction(EuqipSubAction, ETriggerEvent::Triggered, this, &UPTInputComponent::OnEquipSubInput);
}

void UPTInputComponent::SetCharacterControl(ECharacterControlType NewType)
{
	UPTCharacterControlData* NewCharacterControlData = CharacterControlMap[NewType];
	check(NewCharacterControlData);
	
	ApplyCharacterControlData(NewCharacterControlData);
	
	APlayerController* PlayerController = CastChecked<APlayerController>(Character->GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		CurrentCharacterControlType = NewType;

		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControlData->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
}

void UPTInputComponent::ApplyCharacterControlData(const UPTCharacterControlData* CharacterControlData)
{
	// Pawn
	Character->bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw; //언체크 (체크)
	
	// CharacterMovement
	MovementComponent->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement; //체크   (Shooter: 언체크)
	MovementComponent->bUseControllerDesiredRotation= CharacterControlData->bUseControllerDesiredRotation; //언체크  (언체크)
	MovementComponent->RotationRate = CharacterControlData->RotationRate; //0, 0 720 (0 0 360)
	
	SpringArmComponent->TargetArmLength = CharacterControlData->TargetArmLength; //300 (300)
	SpringArmComponent->SetRelativeLocation(CharacterControlData->RelativeLocation); //0 70 70 
	SpringArmComponent->SetRelativeRotation(CharacterControlData->RelativeRotation); //0 0 0
	SpringArmComponent->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation; //체크 (체크)
	SpringArmComponent->bInheritPitch = CharacterControlData->bInheritPitch; //체크 (체크)
	SpringArmComponent->bInheritYaw = CharacterControlData->bInheritYaw; //체크 (체크)
	SpringArmComponent->bInheritRoll = CharacterControlData->bInheritRoll; //체크 (체크)
	SpringArmComponent->bDoCollisionTest = CharacterControlData->bDoCollisionTest; //체크 (체크)
}

void UPTInputComponent::ChangeCharacterControl(ECharacterControlType Type)
{
	switch (Type)
	{
		case ECharacterControlType::Quater:
			SetCharacterControl(ECharacterControlType::Quater);
			return;

		case ECharacterControlType::Zoom:
			SetCharacterControl(ECharacterControlType::Zoom);
			return;

		case ECharacterControlType::Shoulder:
			SetCharacterControl(ECharacterControlType::Shoulder);
			return;
	}
}

void UPTInputComponent::OnShoulderMoveInput(const FInputActionValue& Value)
{
	// Look만 DeltaSeconds를 적용한 이유 - 이동 내부로직에서, 인자로 받은 값에 따라 처리하고 있음 (인자가 이동거리가 아닌 속도의 개념)
	// 멀티에서는 서버에서 이를 조정, 로컬은 회전만 감도설정과 DeltaTime에 따라 조정하면 된다.  
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = Character->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	Character->AddMovementInput(ForwardDirection, MovementVector.X);
	Character->AddMovementInput(RightDirection, MovementVector.Y);
}

void UPTInputComponent::OnShoulderLookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	LookAxisVector *= RotationRate * GetWorld()->GetDeltaSeconds();
	
	// 반대로 움직이는 이유 찾지 못함, 일단 - 로 
	Character->AddControllerYawInput(-LookAxisVector.X);
	Character->AddControllerPitchInput(LookAxisVector.Y);
}

void UPTInputComponent::OnQuaterMoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}
	
	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	Character->GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	Character->AddMovementInput(MoveDirection, MovementVectorSize);
}

void UPTInputComponent::OnDashInput()
{
	IPTPlayerInputInterface* PlayerInputInterface = Cast<IPTPlayerInputInterface>(Character);	
	if (PlayerInputInterface)
	{
		PlayerInputInterface->DashAction();
	}
}

void UPTInputComponent::OnStartAttackInput()
{
	IPTPlayerInputInterface* PlayerInputInterface = Cast<IPTPlayerInputInterface>(Character);
	if (PlayerInputInterface)
	{
		PlayerInputInterface->StartAttack();
	}
}

void UPTInputComponent::OnCompleteAttackInout()
{
	IPTPlayerInputInterface* PlayerInputInterface = Cast<IPTPlayerInputInterface>(Character);	
	if (PlayerInputInterface)
	{
		PlayerInputInterface->StopAttack();
	}
}

void UPTInputComponent::OnReloadInput()
{
	IPTPlayerInputInterface* PlayerInputInterface = Cast<IPTPlayerInputInterface>(Character);	
	if (PlayerInputInterface)
	{
		PlayerInputInterface->ReloadAction();
	}
}

void UPTInputComponent::OnZoomInInput()
{
	ChangeCharacterControl(ECharacterControlType::Zoom);
}

void UPTInputComponent::OnZoomOutInput()
{
	ChangeCharacterControl(ECharacterControlType::Shoulder);
}

void UPTInputComponent::OnEquipMainInput()
{
	IPTPlayerInputInterface* PlayerInputInterface = Cast<IPTPlayerInputInterface>(Character);	
	if (PlayerInputInterface)
	{
		PlayerInputInterface->EquipInput(EEquipType::Main);
	}		
}

void UPTInputComponent::OnEquipSubInput()
{
	IPTPlayerInputInterface* PlayerInputInterface = Cast<IPTPlayerInputInterface>(Character);	
	if (PlayerInputInterface)
	{
		PlayerInputInterface->EquipInput(EEquipType::Sub);
	}			
}
