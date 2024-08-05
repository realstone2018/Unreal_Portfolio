#pragma once

#include "CoreMinimal.h"
#include "PTComponent/Character//PTCharacterStatComponent.h"
#include "GameData/PTMonsterStat.h"
#include "PTMonsterStatComponent.generated.h"

UCLASS()
class PROJECT2_API UPTMonsterStatComponent : public UPTCharacterStatComponent
{
	GENERATED_BODY()

public:
	void SetMonsterStat(FName MonsterDataKey);

	FORCEINLINE const FPTMonsterStat& GetMonsterStat() const { return MonsterStat; }
	FORCEINLINE float GetAttackDamage() const { return MonsterStat.Attack; }	
	FORCEINLINE float GetAttackRange() const { return MonsterStat.AttackRange; }	
	FORCEINLINE float GetAttackRadius() const { return MonsterStat.AttackRadius; }
	FORCEINLINE float GetAttackCooldown() const { return MonsterStat.AttackCooldown; }
	
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPTMonsterStat MonsterStat;
};
