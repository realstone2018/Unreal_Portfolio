#pragma once

#include "CoreMinimal.h"
#include "PTComponent/Character/PTCharacterStatComponent.h"
#include "GameData/PTCharacterStat.h"
#include "PTPlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FPTCharacterStat& /*BaseStat*/, const FPTCharacterStat& /*ModifierStat*/)

UCLASS()
class PROJECT2_API UPTPlayerStatComponent : public UPTCharacterStatComponent
{
	GENERATED_BODY()

public:
	UPTPlayerStatComponent();

	virtual void InitializeComponent() override;
	
	FOnStatChangedDelegate OnStatChanged;

	void SetCharacterLevelStat(int32 InNewLevel);
	
	void SetBaseStat(const FPTCharacterStat& InBaseStat);
	void SetModifierStat(const FPTCharacterStat& InModifierStat);
	void AddBaseStat(const FPTCharacterStat& InAddBaseStat);

	FORCEINLINE FPTCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE const FPTCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FPTCharacterStat& GetModifierStat() const { return ModifierStat; }
	
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPTCharacterStat BaseStat;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPTCharacterStat ModifierStat;	
};
