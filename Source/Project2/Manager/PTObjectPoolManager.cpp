#include "Manager/PTObjectPoolManager.h"
#include "Interface/PTPullingObjectInterface.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "Engine/AssetManager.h"
#include "GameData/ObjectPoolData.h"

UPTObjectPoolManager::UPTObjectPoolManager()
{
	static ConstructorHelpers::FObjectFinder<UObjectPoolData> PoolDataRef(TEXT("/Script/Project2.ObjectPoolData'/Game/Project2/GameData/ObjectPoolDataAsset.ObjectPoolDataAsset'"));
	ensure(PoolDataRef.Object);
	if (PoolDataRef.Object)
	{
		PoolData = PoolDataRef.Object;
	}
	const FPoolData Data = PoolData->GetPoolData(EPoolType::Monster);
	UE_LOG(LogTemp, Display, TEXT("SetupCount: %d"), Data.SetupSize);
		
}

void UPTObjectPoolManager::Init(UWorld* World)
{
	WorldContext = World;


	// UAssetManager& Manager = UAssetManager::Get();
	//
	// TArray<FPrimaryAssetId> Assets;
	// Manager.GetPrimaryAssetIdList(TEXT("PTPoolData"), Assets);
	// ensure(0 < Assets.Num());
	// FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[0]));
	//
	// //애셋이 로드되어 있지 않다면 로드 
	// {
	// 	AssetPtr.LoadSynchronous();
	// }
	//
	// PoolData = Cast<UObjectPoolData>(AssetPtr.Get());
}

template <typename T, typename>
void UPTObjectPoolManager::SetUpPool(EPoolType PoolType, /*TSubclassOf<T> ObjectClass,*/ int32 SetUpSize)
{
	//TODO: Path는 Data에서 Size와 Class와 같이 받아서 사용하기 
	FString ParentName = UEnum::GetValueAsString(PoolType);
	FString ParentPath = "ObjectPool/" + ParentName;
	
	if (!PoolMap.Contains(PoolType))
	{
		PoolMap.Add(PoolType, FPoolArray());
	}

	ensure(PoolData);
	const FPoolData Data = PoolData->GetPoolData(PoolType);
	int Size = SetUpSize != 0 ? SetUpSize : Data.SetupSize;
	
	for (int32 i = 0; i < Size; i++)
	{
		AActor* NewObject = WorldContext->SpawnActor<T>(Data.PoolClass);
		check(NewObject);
		
		NewObject->SetFolderPath(*ParentPath);
		if (IPTPullingObjectInterface* PoolableObject = Cast<IPTPullingObjectInterface>(NewObject))
		{
			PoolableObject->Terminate();
			PoolMap[PoolType].DeactiveArray.Add(NewObject);
		}
	}
}

template <typename T, typename>
T* UPTObjectPoolManager::GetPooledObject(EPoolType PoolType, FTransform const& Trans)
{
	if (!PoolMap.Contains(PoolType))
	{
		return nullptr;
	}

	if (PoolMap[PoolType].DeactiveArray.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("%s PoolType is Full, Increase Pull Size"), *UEnum::GetValueAsString(PoolType));
		
		SetUpPool<T>(PoolType, PoolMap[PoolType].ActiveArray.Num() / 2);
	}
	
	AActor* Object = PoolMap[PoolType].DeactiveArray[0];
	PoolMap[PoolType].DeactiveArray.RemoveAt(0);
	PoolMap[PoolType].ActiveArray.Add(Object);

	Object->SetActorTransform(Trans);

	IPTPullingObjectInterface* InterfaceObject = Cast<IPTPullingObjectInterface>(Object);
	InterfaceObject->Initialize();

	T* PoolableObject = Cast<T>(Object);
	check(PoolableObject);
	
	return PoolableObject;
}

void UPTObjectPoolManager::ReturnPooledObject(EPoolType PoolType, AActor* Object)
{
	if (IPTPullingObjectInterface* PoolableObject = Cast<IPTPullingObjectInterface>(Object))
	{
		UE_LOG(LogTemp, Log, TEXT("UPTObjectPoolManager::ReturnPooledObject - Dispose %s"), *Object->GetActorNameOrLabel());

		PoolableObject->Terminate();

		PoolMap[PoolType].DeactiveArray.Add(Object);
		PoolMap[PoolType].ActiveArray.Remove(Object);
	}
}