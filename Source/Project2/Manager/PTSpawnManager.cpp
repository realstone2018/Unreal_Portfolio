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
T* UPTSpawnManager::SpawnObject(FRotator SpawnRotator, FVector SpawnLocation)
{
	FTransform SpawnTransform(SpawnRotator, SpawnLocation);

	T* PooledObject = PoolManager->GetPooledObject<T>(SpawnTransform);
	ensure(PooledObject);

	PooledObject->OnDead.BindUObject(this, &UPTSpawnManager::ReturnObject<T>);

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
		if (NavSystem->GetRandomPointInNavigableRadius(BaseSpawnLocation, NAVIGATION_RANDOM_RADIUS, NavLocation))
		{
			SpawnLocation = NavLocation;
		}
		
		SpawnObject<APTMonster>(FRotator::ZeroRotator, SpawnLocation);
	}	
}
