#pragma once

#include "CoreMinimal.h"
#include "Character/PTMonster.h"
#include "Engine/DataAsset.h"
#include "ObjectPoolData.generated.h"

UENUM(BlueprintType)
enum class EPoolType : uint8
{
	None = 0,
	Monster UMETA(DisplayName = "Monster"),
	Projectile UMETA(DisplayName = "Projectile"),
};

USTRUCT()
struct FPoolData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PoolClass;

	UPROPERTY(EditAnywhere)
	int SetupSize;
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
