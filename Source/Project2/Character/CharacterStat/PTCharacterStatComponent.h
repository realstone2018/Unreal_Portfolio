// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/PTCharacterStat.h"
#include "PTCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FPTCharacterStat& /*BaseStat*/, const FPTCharacterStat& /*ModifierStat*/)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT2_API UPTCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPTCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;
	
public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;
	
	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE FPTCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }

	FORCEINLINE void SetBaseStat(const FPTCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetModifierStat(const FPTCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void AddBaseStat(const FPTCharacterStat& InAddBaseStat) { BaseStat = BaseStat + InAddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());}
	
	FORCEINLINE const FPTCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FPTCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }	

protected:
	void SetHp(float NewHp);

public:
	FORCEINLINE void HealHp(float InHealAmount) { CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp); }
	float ApplyDamage(float InDamage);

protected:
	//Transient: 항상 바뀌는 값이라 디스크에 저장할 필요 없는 경우 세팅
	//TODO: float여야 할 이유 없으면 int로 변경하기 
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPTCharacterStat BaseStat;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPTCharacterStat ModifierStat;
};
