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
	void Init(UWorld* Inworld, UPTObjectPoolManager* InPoolManager);

#pragma region Polling
public:
	template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
T* SpawnObject(FRotator SpawnRotator, FVector SpawnLocation);

	template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
	void ReturnObject(AActor* PooledObject);

private:
	UPROPERTY()
	TObjectPtr<class UWorld> World;
	
	UPROPERTY()
	TObjectPtr<UPTObjectPoolManager> PoolManager;

	const float SPAWN_RADIUS = 8000.f;
	const float NAVIGATION_RANDOM_RADIUS = 500.f;
#pragma endregion


#pragma region Monster
	void SpawnMonsterWave(FVector BaseSpawnLocation, int32 Num);


#pragma endregion
};
