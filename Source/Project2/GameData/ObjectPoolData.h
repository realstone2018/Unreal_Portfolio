// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PTMonster.h"
#include "Engine/DataAsset.h"
#include "ObjectPoolData.generated.h"

UCLASS()
class PROJECT2_API UObjectPoolData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("PTPoolData", GetFName());
	}

	UPROPERTY(EditAnywhere)
	TMap<TSoftClassPtr<APTMonster>, int32> PoolData;
};
