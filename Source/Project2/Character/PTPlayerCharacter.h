#pragma once

#include "CoreMinimal.h"
#include "Character/PTCharacterBase.h"
#include "PTComponent/PTInputComponent.h"
#include "Interface/PTPlayerInputInterface.h"
#include "Character/CharacterStat/PTCharacterStatComponent.h"
#include "Weapon/Gun.h"
#include "PTPlayerCharacter.generated.h"

UCLASS()
class PROJECT2_API APTPlayerCharacter : public APTCharacterBase, public IPTPlayerInputInterface
{
	GENERATED_BODY()

public:
	APTPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	
// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UCameraComponent> FollowCamera;
	
// Input
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UPTInputComponent> PlayerInputComponent;
	
// Stat
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPTPlayerStatComponent> StatComponent;

	virtual UPTCharacterStatComponent* GetStatComponent() override;

// Widget
	virtual bool GetShouldDisplayHpBar() override { return false; }
	
public:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Evation() override;
	
// IPTPlayerInputInterface
	void StartAttack() override;
	void StopAttack() override;
	void EvationAction() override { Evation(); }

	virtual void Dead() override;
	
// Weapon
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> GunClass;

	UPROPERTY()
	AGun* Gun;

	void Reloading();
	
	bool bIsReloading;
	
};
