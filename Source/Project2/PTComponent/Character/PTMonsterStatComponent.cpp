#include "PTComponent/Character/PTMonsterStatComponent.h"
#include "GameData/PTGameDataSingleton.h"
#include "GameData/PTMonsterStat.h"

void UPTMonsterStatComponent::SetMonsterStat(FName MonsterDataKey)
{
	MonsterStat = UPTGameDataSingleton::Get().GetMonsterStat(MonsterDataKey);

	SetHp(MonsterStat.MaxHp, MonsterStat.MaxHp);

	//TODO: Hp외의 컬럼들도 세팅
	
}
