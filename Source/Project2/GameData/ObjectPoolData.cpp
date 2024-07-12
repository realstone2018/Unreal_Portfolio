// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ObjectPoolData.h"

const FPoolData& UObjectPoolData::GetPoolData(EPoolType PoolType)
{
	return PoolData[PoolType];
}
