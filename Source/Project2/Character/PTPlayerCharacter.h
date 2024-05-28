#pragma once

#include "CoreMinimal.h"
#include "Character/PTCharacterBase.h"
#include "PTInputComponent.h"
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
	
// Camera Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UCameraComponent> FollowCamera;
	

// Input Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPTInputComponent* PlayerInputComponent;
	
public:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IPTPlayerInputInterface
	virtual void StartAttack() override;
	virtual void StopAttack() override;

// Weapon
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> GunClass;

	UPROPERTY()
	AGun* Gun;

	void Reloading();
	
	bool bIsReloading;




	
};
