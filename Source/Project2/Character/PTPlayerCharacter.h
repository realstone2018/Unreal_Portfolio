#pragma once

#include "CoreMinimal.h"
#include "Character/PTCharacterBase.h"
#include "PTInterface/PTPlayerInputInterface.h"
#include "PTInterface/PTCharactHUDInterface.h"
#include "PTPlayerCharacter.generated.h"

UCLASS()
class PROJECT2_API APTPlayerCharacter : public APTCharacterBase, public IPTPlayerInputInterface, public IPTCharactHUDInterface
{
	GENERATED_BODY()

public:
	APTPlayerCharacter();

private:
	virtual void PostInitializeComponents() override;
	
	virtual void PossessedBy(AController* NewController) override;
	
// Camera
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UCameraComponent> FollowCamera;
	
// Input
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UPTInputComponent> PlayerInputComponent;

public:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Dash() override;
	
// Battle
	void StartAttack() override;
	void StopAttack() override;

	//TODO: AI의 Evation을 위해 이렇게 해둔거 같긴한데.. 필요한가?
	void DashAction() override { Dash(); }
	void ReloadAction() override;

	void EquipInput(EEquipType EquipType) override;
	
	virtual void Dead() override;

// Stat
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPTPlayerStatComponent> StatComponent;

	virtual UPTCharacterStatComponent* GetStatComponent() override;
	
// Weapon
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class UPTEquipmentComponent> EquipmentComponent;

	void Reloading();
	
	UPROPERTY()
	bool IsReloading;

// UI Section
private:
	// Widget
	virtual bool GetShouldDisplayHpBar() override { return false; }
	
	// IPTCharacterHUDInterface
	virtual void SetupHUDWidget(UPTHUDWidget* InHUDWidget) override;

	
	
};
