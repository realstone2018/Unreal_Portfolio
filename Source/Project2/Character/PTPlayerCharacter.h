#pragma once

#include "CoreMinimal.h"
#include "Character/PTCharacterBase.h"
#include "PTInterface/PTPlayerInputInterface.h"
#include "PTPlayerCharacter.generated.h"

UCLASS()
class PROJECT2_API APTPlayerCharacter : public APTCharacterBase, public IPTPlayerInputInterface
{
	GENERATED_BODY()

public:
	APTPlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region Component
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UPTInputComponent> PlayerInputComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPTPlayerStatComponent> StatComponent;

	virtual UPTCharacterStatComponent* GetStatComponent() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class UPTEquipmentComponent> EquipmentComponent;

#pragma endregion 


#pragma region Battle
public:
	virtual void Dead() override;
	virtual void Dash();
	
	void StartAttack() override;
	void StopAttack() override;

	void DashAction() override { Dash(); }
	void ReloadAction() override;

	void EquipInput(EEquipType EquipType) override;
	
	void Reloading();

private:
	UPROPERTY()
	uint8 IsReloading : 1;

#pragma endregion


#pragma region Widget
public:
	virtual uint8 GetShouldDisplayHpBar() override { return false; }
	
private:
	virtual void SetupHUDWidget(UPTHUDWidget* InHUDWidget) override; // IPTCharacterHUDInterface
	
};
