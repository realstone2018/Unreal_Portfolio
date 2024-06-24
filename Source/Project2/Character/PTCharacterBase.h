// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/PTCharacterStatComponent.h"
#include "GameFramework/Character.h"
#include "Interface/PTAnimationAttackInterface.h"
#include "Interface/PTCharacterWidgetInterface.h"
#include "UI/PTWidgetComponent.h"
#include "UI/PTUserWidget.h"
#include "PTCharacterBase.generated.h"

UCLASS()
class PROJECT2_API APTCharacterBase : public ACharacter, public IPTAnimationAttackInterface, public IPTCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	APTCharacterBase();

	virtual void PostInitializeComponents() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", Meta = (AllowPrivateAceess = "true"))
	TObjectPtr<class UPTCharacterMoveComponent> MoveComponent;

	virtual UPTCharacterStatComponent* GetStatComponent();

	// Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPTWidgetComponent> HpBar; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPTUserWidget> HpBarWidgetClass;
	
	//virtual void SetupCharacterWidget(class UPTUserWidget* InUserWidget) override;	

	virtual void SetupCharacterWidget(UPTUserWidget* InUserWidget) override;
	virtual bool GetShouldDisplayHpBar() { return true; }
	
public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Evation();

	virtual void OnNotifyAttack() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	virtual bool IsDead();

	virtual void Kill(AActor* victim);
	virtual void Dead();

	
};
