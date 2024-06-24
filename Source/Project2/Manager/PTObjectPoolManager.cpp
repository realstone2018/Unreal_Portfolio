#include "Manager/PTObjectPoolManager.h"
#include "Interface/PTPullingObjectInterface.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "Engine/AssetManager.h"
#include "GameData/ObjectPoolData.h"

UPTObjectPoolManager::UPTObjectPoolManager()
{
	UE_LOG(LogTemp, Display, TEXT("UPTObjectPoolManager::UPTObjectPoolManager"));
}

void UPTObjectPoolManager::Init(UWorld* World)
{
	UE_LOG(LogTemp, Display, TEXT("UPTObjectPoolManager::Init"));
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
void UPTObjectPoolManager::InitializePool(EPoolListType PoolType, TSubclassOf<T> ObjectClass, int32 InitialSize)
{
	UE_LOG(LogTemp, Display, TEXT("UPTObjectPoolManager::InitializePool()"));

	FString ParentName = UEnum::GetValueAsString(PoolType);

	//TODO: Path는 Data에서 Size와 Class와 같이 받아서 사용하기 
	FString ParentPath = "ObjectPool/" + ParentName;
	
	if (PoolMap.IsEmpty() || !PoolMap.Contains(PoolType))
	{
		AActor*  ParentActor = GetWorld()->SpawnActor<AActor>();

		if (ParentActor) {
			ParentActor->SetFolderPath(TEXT("ObjectPool"));
			ParentActor->SetActorLabel(ParentName);
			
			PoolMap.Add(PoolType, FPoolArray(ParentActor));
		}
	}
	
	for (int32 i = 0; i < InitialSize; ++i)
	{
		AActor* NewObject = WorldContext->SpawnActor<AActor>(ObjectClass);
		if (NewObject == nullptr)
		{
			return;
		}
		
		NewObject->SetFolderPath(*ParentPath);

		//TODO: 인터페이스 사용 방식 검토 
		if (IPTPullingObjectInterface* PoolableObject = Cast<IPTPullingObjectInterface>(NewObject))
		{
			PoolableObject->Dispose();
			PoolMap[PoolType].PoolArray.Add(NewObject);
		}
	}
}

template <typename T, typename>
T* UPTObjectPoolManager::GetPooledObject(EPoolListType PoolType, FTransform const& Trans)
{
	if (!PoolMap.Contains(PoolType))
	{
		return nullptr;
	}
	
	for (AActor* Object : PoolMap[PoolType].PoolArray)
	{
		if (!Object->IsHidden())
		{
			continue;
		}

		if (IPTPullingObjectInterface* InterfaceObject = Cast<IPTPullingObjectInterface>(Object))
		{
			InterfaceObject->Instantiate();

			T* PoolableObject = Cast<T>(Object);
			if (PoolableObject)
			{
				Object->SetActorTransform(Trans);
				return PoolableObject;
			}
		}
	}

	//TODO: 풀링해둔게 모자라면 하나씩하는게 아니고 여유분을 한번에 
	// // If no inactive object is found, spawn a new one
	// T* NewObject = WorldContext->SpawnActor<T>(PooledObjectClass);
	// if (NewObject)
	// {
	// 	NewObject->Activate();
	// 	PoolMap[PoolType].PoolArray(NewObject);
	// 	return NewObject;
	// }

	return nullptr;
}

void UPTObjectPoolManager::ReturnPooledObject(AActor* Object)
{
	if (IPTPullingObjectInterface* PoolableObject = Cast<IPTPullingObjectInterface>(Object))
	{
		UE_LOG(LogTemp, Log, TEXT("UPTObjectPoolManager::ReturnPooledObject - Dispose %s"), *Object->GetActorNameOrLabel());

		PoolableObject->Dispose();
	}
}