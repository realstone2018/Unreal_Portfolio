#include "GameData/ObjectPoolData.h"

const FPoolData& UObjectPoolData::GetPoolData(EPoolType PoolType)
{
	return PoolData[PoolType];
}
