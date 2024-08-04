#include "Manager//PTAssetManager.h"

#include "Particles/ParticleSystem.h"

DEFINE_LOG_CATEGORY(LogPTAssetManager);

void UPTAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	StreamableMgr = &GetStreamableManager();
}

UPTAssetManager& UPTAssetManager::Get()
{
	UPTAssetManager* AssetManager = CastChecked<UPTAssetManager>(GEngine->AssetManager);
	if (AssetManager)
	{
		return *AssetManager;
	}

	UE_LOG(LogPTAssetManager, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UPTAssetManager>();
}


template <typename T>
void UPTAssetManager::LoadMeshAsset(FString Name, TFunction<void(T*)> OnLoadComplete)
{
	if (Name.IsEmpty())
	{
		return;
	}
	
	FString Path = FString::Format(*MESH_PATH_FORMAT, { Name });
	UE_LOG(LogTemp, Log, TEXT("UPTAssetManager::LoadMeshAsset() - Path: %s"), *Path);

	LoadAsset(Path, OnLoadComplete);
}

template <typename T>
void UPTAssetManager::LoadFXAsset(FString Name, TFunction<void(T*)> OnLoadComplete)
{
	if (Name.IsEmpty())
	{
		return;
	}
	
	FString Path = FString::Format(*FX_PATH_FORMAT, { Name });
	UE_LOG(LogTemp, Log, TEXT("UPTAssetManager::LoadFXAsset() - Path: %s"), *Path);

	LoadAsset(Path, OnLoadComplete);
}

template <typename T>
void UPTAssetManager::LoadSFXAsset(FString Name, TFunction<void(T*)> OnLoadComplete)
{
	if (Name.IsEmpty())
	{
		return;
	}
	
	FString Path = FString::Format(*SFX_PATH_FORMAT, { Name });
	UE_LOG(LogTemp, Log, TEXT("UPTAssetManager::LoadSFXAsset() - Path: %s"), *Path);

	LoadAsset(Path, OnLoadComplete);	
}

template <typename T>
void UPTAssetManager::LoadAsset(FString Path, TFunction<void(T*)> OnLoadComplete)
{
	FSoftObjectPath AssetPath(Path);
	if (!AssetPath.IsValid())
	{
		UE_LOG(LogPTAssetManager, Error, TEXT("Asset Load Fail, InValid Path - Path: %s"), *Path);
	}
 	
	TSharedPtr<FStreamableHandle> AssetHandle = StreamableMgr->RequestAsyncLoad(AssetPath, FStreamableDelegate::CreateLambda([this, AssetPath, OnLoadComplete]() {
		TSoftObjectPtr<UObject> SoftPtr(AssetPath);
		UObject* LoadedAsset = SoftPtr.Get();
 		
		if (T* Result = Cast<T>(LoadedAsset))
		{
			if (OnLoadComplete)
			{
				OnLoadComplete(Result);
			}	
		}
	}));
}

template <typename T>
void UPTAssetManager::LoadAssets(TArray<FString> AssetPaths, TFunction<void(TArray<UObject*>)> OnLoadComplete)
{
	TArray<FSoftObjectPath> SoftObjectPaths;
	for (const FString Path : AssetPaths)
	{
		SoftObjectPaths.Add(FSoftObjectPath(Path));
	}

	TSharedPtr<FStreamableHandle> AssetHandle = StreamableManager.RequestAsyncLoad(SoftObjectPaths, FStreamableDelegate::CreateLambda([this, AssetPaths, OnLoadComplete]()
	{
		UE_LOG(LogTemp, Log, TEXT("Assets have been loaded asynchronously."));

		TArray<T*> LoadedAssets;
		for (const auto& Path : AssetPaths)
		{
			TSoftObjectPtr SoftPtr(Path);
			UObject* LoadedObject = SoftPtr.Get();

			if (T* Result = Cast<T>(LoadedObject))
			{
				LoadedAssets.Add(Result);
			}
		}

		if (OnLoadComplete)
		{
			OnLoadComplete(LoadedAssets);
		}
	}));
}
