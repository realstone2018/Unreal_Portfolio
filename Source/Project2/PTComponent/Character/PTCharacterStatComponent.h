#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PTCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, int32 /*CurrentHp*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT2_API UPTCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPTCharacterStatComponent();
	
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	
	FORCEINLINE int32 GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE int32 GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE int32 GetMaxHp() const { return MaxHp; }

protected:
	void SetHp(int32 NewHp, int32 NewMaxHp);

public:
	FORCEINLINE void HealHp(int32 InHealAmount) { SetHp(CurrentHp + InHealAmount, MaxHp); }
	virtual float ApplyDamage(int32 InDamage);

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentLevel;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 MaxHp;

};
