#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PTInterface/PTAnimationInterface.h"
#include "PTInterface/PTHUDInterface.h"
#include "PTComponent/Character/PTCharacterStatComponent.h"
#include "PTInterface/PTFactionInterface.h"
#include "PTCharacterBase.generated.h"

UCLASS()
class PROJECT2_API APTCharacterBase : public ACharacter, public IPTAnimationInterface, public IPTHUDInterface, public IPTFactionInterface
{
	GENERATED_BODY()

public:
	APTCharacterBase();

	virtual void PostInitializeComponents() override;


#pragma region Component
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class UPTCharacterMoveComponent> MoveComponent;
	
	virtual UPTCharacterStatComponent* GetStatComponent() { return nullptr; }

#pragma endregion


#pragma region Battle
public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Kill(AActor* victim) { }
	virtual void Dead();
	
	UFUNCTION(BlueprintPure)
	virtual bool IsDead() { return GetStatComponent()->GetCurrentHp() <= 0; }

	virtual void OnNotifyAttack() override { }

	virtual EFaction GetFaction() override { return EFaction::None; }
#pragma endregion


#pragma region Widget
public:
	virtual void SetupHUDWidget(UPTHUDWidget* InHUDWidget) override { }
	virtual void SetupHpBarWidget(UPTUserWidget* InUserWidget) override;
	virtual uint8 GetShouldDisplayHpBar() { return true; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPTWidgetComponent> HpBar; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPTUserWidget> HpBarWidgetClass;

#pragma endregion
	
};
