#include "PTComponent/Character/PTMonsterStatComponent.h"
#include "GameData/PTGameDataSingleton.h"
#include "GameData/PTMonsterStat.h"

void UPTMonsterStatComponent::SetMonsterStat(FName MonsterName)
{
	MonsterStat = UPTGameDataSingleton::Get().GetMonsterStat(MonsterName);

	SetHp(MonsterStat.MaxHp, MonsterStat.MaxHp);

	//TODO: Hp외의 컬럼들도 세팅
	
}
