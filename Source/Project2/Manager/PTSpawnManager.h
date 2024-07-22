#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Manager/PTObjectPoolManager.h"
#include "PTSpawnManager.generated.h"

UCLASS()
class PROJECT2_API UPTSpawnManager : public UObject
{
	GENERATED_BODY()

public:
	UPTSpawnManager();

	void Init(UPTObjectPoolManager* InPoolManager);
	
	template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
	T* SpawnObject(FRotator SpawnRotator, FVector SpawnLocation);

	template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
	void ReturnObject(AActor* PooledObject);

	void SpawnMonsterWave(FVector SpawnLocation, int32 Num);

private:
	UPROPERTY()
	UPTObjectPoolManager* PoolManager;

	UPROPERTY()
	TArray<FVector2D> SpawnLocationOffsets;

	const int GroupSize = 30;
	const int GroupColumnSize = 6;
};
