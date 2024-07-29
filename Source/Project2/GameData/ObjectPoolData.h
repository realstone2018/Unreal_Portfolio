#pragma once

#include "CoreMinimal.h"
#include "Enum/ObjectPoolType.h"
#include "Engine/DataAsset.h"
#include "ObjectPoolData.generated.h"

USTRUCT()
struct FPoolData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PoolClass;

	UPROPERTY(EditAnywhere)
	int32 SetupSize;
	FPoolData operator*();
};

UCLASS()
class PROJECT2_API UObjectPoolData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("PTPoolData", GetFName());
	}

	const FPoolData& GetPoolData(EPoolType PoolType);

	UPROPERTY(EditAnywhere)
	TMap<EPoolType, FPoolData> PoolData;

};
