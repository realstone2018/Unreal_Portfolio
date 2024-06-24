#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterStat/PTCharacterStatComponent.h"
#include "PTPlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FPTCharacterStat& /*BaseStat*/, const FPTCharacterStat& /*ModifierStat*/)

UCLASS()
class PROJECT2_API UPTPlayerStatComponent : public UPTCharacterStatComponent
{
	GENERATED_BODY()

public:
	UPTPlayerStatComponent();
	
	FOnStatChangedDelegate OnStatChanged;

	void SetCharacterLevelStat(int32 InNewLevel);
	
	FORCEINLINE void SetBaseStat(const FPTCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetModifierStat(const FPTCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void AddBaseStat(const FPTCharacterStat& InAddBaseStat) { BaseStat = BaseStat + InAddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());}

	FORCEINLINE FPTCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE const FPTCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FPTCharacterStat& GetModifierStat() const { return ModifierStat; }
	
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPTCharacterStat BaseStat;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPTCharacterStat ModifierStat;	
};
