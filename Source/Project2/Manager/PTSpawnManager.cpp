#include "Manager/PTSpawnManager.h"
#include "NavigationSystem.h"
#include "PTInterface/PTGameInterface.h"
#include "Util/PTVectorUtil.h"

void UPTSpawnManager::Init(UWorld* InWorld, UPTObjectPoolManager* InPoolManager)
{
	World = InWorld;
	PoolManager = InPoolManager;
}

template <typename T, typename>
T* UPTSpawnManager::SpawnObject(FName DataKey, FRotator SpawnRotator, FVector SpawnLocation, uint8 ReturnImmediately)
{
	FTransform SpawnTransform(SpawnRotator, SpawnLocation);

	T* PooledObject = PoolManager->GetPoolObject<T>(SpawnTransform, DataKey);
	ensure(PooledObject);

	if (ReturnImmediately)
	{
		PooledObject->OnDead.BindUObject(this, &UPTSpawnManager::ReturnImmediatelyObject<T>);
	}
	else
	{
		PooledObject->OnDead.BindUObject(this, &UPTSpawnManager::ReturnObject<T>);
	}
	
	return PooledObject;
}

template <typename T, typename>
void UPTSpawnManager::ReturnObject(AActor* PooledObject)
{
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle,
		FTimerDelegate::CreateLambda([this, PooledObject](){
			PoolManager->ReturnPoolObject<T>(PooledObject);
		}
	), 5.0f, false);
}

template <typename T, typename>
void UPTSpawnManager::ReturnImmediatelyObject(AActor* PooledObject)
{
	PoolManager->ReturnPoolObject<T>(PooledObject);
}

void UPTSpawnManager::SpawnMonsterWave(FVector BaseSpawnLocation, int32 Num)
{
	for (int i = 0; i < Num; i++)
	{
		FVector SpawnLocation = PTVectorUtil::GetCirclePoint<double>(BaseSpawnLocation, SPAWN_RADIUS);
		SpawnLocation += FVector(0.f, 0.f, 500.f);
		SpawnObject<APTMonster>(FName("Scorch"), FRotator::ZeroRotator, SpawnLocation, false);
	}	
}
