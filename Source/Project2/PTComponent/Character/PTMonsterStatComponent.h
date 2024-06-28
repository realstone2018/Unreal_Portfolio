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
	UPTMonsterStatComponent();

	//TODO: Enum으로 관리 
	void SetMonsterStat(FName MonsterName);

	FORCEINLINE const FPTMonsterStat& GetMonsterStat() const { return MonsterStat; }
	
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPTMonsterStat MonsterStat;
};
