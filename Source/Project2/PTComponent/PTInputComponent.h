#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "PTInputComponent.generated.h"

UENUM()
enum class ECharacterControlType : uint8 {
	Shoulder,
	Quater
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT2_API UPTInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPTInputComponent();

	virtual void BeginPlay() override;

	void Init(USpringArmComponent* SpringArm);
	void SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent);
	void SetCharacterControl(ECharacterControlType NewType);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACharacter* PlayerCharacter;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY()
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UPTCharacterControlData*> CharacterControlMap;

	void ApplyCharacterControlData(const class UPTCharacterControlData* CharacterControlData);
	void ChangeCharacterControl(const ULocalPlayer* LocalPlayer);
	
	ECharacterControlType CurrentCharacterControlType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> ShoulderMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> ShoulderLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> QuaterMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> ShootAction;

	void OnShoulderMoveInput(const FInputActionValue& Value);
	void OnShoulderLookInput(const FInputActionValue& Value);
	void OnQuaterMoveInput(const FInputActionValue& Value);

	void OnJumpInput();
	void OnJumpStopInput();

	void OnStartAttackInput();
	void OnCompleteAttackInout();
	
	//TODO: 옵션에서 감도 설정할 수 있게
	UPROPERTY(EditAnywhere)
	float RotationRate = 60;
};


