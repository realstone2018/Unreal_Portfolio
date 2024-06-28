#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/PTCharacterStat.h"
#include "PTCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, int /*CurrentHp*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT2_API UPTCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPTCharacterStatComponent();
	
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	
	FORCEINLINE int GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE int GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE int GetMaxHp() const { return MaxHp; }
	FORCEINLINE int GetAttackRadius() const { return AttackRadius; }	

protected:
	void SetHp(int NewHp, int NewMaxHp);

public:
	FORCEINLINE void HealHp(int InHealAmount) { SetHp(CurrentHp + InHealAmount, MaxHp); }
	virtual float ApplyDamage(int InDamage);

protected:
	//Transient: 항상 바뀌는 값이라 디스크에 저장할 필요 없는 경우 세팅
	//TODO: float여야 할 이유 없으면 int로 변경하기 
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int CurrentLevel;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int MaxHp;
	
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

};
