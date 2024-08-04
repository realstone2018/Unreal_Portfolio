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
T* UPTSpawnManager::SpawnObject(FRotator SpawnRotator, FVector SpawnLocation, uint8 ReturnImmediately)
{
	FTransform SpawnTransform(SpawnRotator, SpawnLocation);

	T* PooledObject = PoolManager->GetPooledObject<T>(SpawnTransform);
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
			PoolManager->ReturnPooledObject<T>(PooledObject);
		}
	), 5.0f, false);
}

template <typename T, typename>
void UPTSpawnManager::ReturnImmediatelyObject(AActor* PooledObject)
{
	PoolManager->ReturnPooledObject<T>(PooledObject);
}

void UPTSpawnManager::SpawnMonsterWave(FVector BaseSpawnLocation, int32 Num)
{
	BaseSpawnLocation = PTVectorUtil::GetCirclePoint<double>(BaseSpawnLocation, SPAWN_RADIUS);
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(World);
	if (NavSystem == nullptr)
	{ 
		return;
	}
	
	for (int i = 0; i < Num; i++)
	{
		FVector SpawnLocation = BaseSpawnLocation;
		FNavLocation NavLocation;
		// if (NavSystem->GetRandomPointInNavigableRadius(BaseSpawnLocation, NAVIGATION_RANDOM_RADIUS, NavLocation))
		// {
		// 	SpawnLocation = NavLocation;
		// }
		//SpawnObject<APTMonster>(FRotator::ZeroRotator, SpawnLocation, false);

		FVector DebugLocation(6500.f, 470.f, 130.f);
		if (NavSystem->GetRandomPointInNavigableRadius(DebugLocation, 10.F, NavLocation))
		{
			DebugLocation = NavLocation;
		}
		SpawnObject<APTMonster>(FRotator::ZeroRotator, DebugLocation, false);
	}	
}
