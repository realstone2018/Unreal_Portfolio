#include "Manager/PTSpawnManager.h"
#include "PTInterface/PTGameInterface.h"

UPTSpawnManager::UPTSpawnManager()
{
	int Row = GroupSize / GroupColumnSize + 1;
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j <= GroupColumnSize; j++)
		{
			SpawnLocationOffsets.Add(FVector2D(j, i));	
		}
	}
}

void UPTSpawnManager::Init(UPTObjectPoolManager* InPoolManager)
{
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


void UPTSpawnManager::SpawnMonsterWave(FVector SpawnLocation, int32 Num)
{
	//APTGameModeBase* GameMode = Cast<APTGameModeBase>(GetWorld()->GetAuthGameMode());

	for (int i = 0; i < Num; i++)
	{
		FVector2D SpawnLocationOffset2D = SpawnLocationOffsets[i];
		float MonsterSize = 300.f;

		SpawnLocationOffset2D = SpawnLocationOffset2D * MonsterSize;

		FVector SpawnLocationOffset(SpawnLocationOffset2D.X, SpawnLocationOffset2D.Y, 0);
		SpawnLocation += SpawnLocationOffset;
		
		SpawnObject<APTMonster>(FRotator::ZeroRotator, SpawnLocation);
	}	
}
