#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PTAssetManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPTAssetManager, Error, All);

UCLASS()
class PROJECT2_API UPTAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	virtual void StartInitialLoading() override;
	
	static UPTAssetManager& Get();

	template <typename T>
	void LoadMeshAsset(FString Name, TFunction<void(T*)> OnLoadComplete);
	
	template <typename T>
	void LoadFXAsset(FString Name, TFunction<void(T*)> OnLoadComplete);

	template <typename T>
	void LoadSFXAsset(FString Name, TFunction<void(T*)> OnLoadComplete);
	
    template <typename T>
	void LoadAsset(FString Path, TFunction<void(T*)> OnLoadComplete);

	template <typename T>
	void LoadAssets(TArray<FString> AssetPaths, TFunction<void(TArray<UObject*>)> OnLoadComplete);
	
private:
	const FString MESH_PATH_FORMAT = TEXT("/Game/Project2/Mesh/{0}.{0}");
	const FString FX_PATH_FORMAT = TEXT("/Game/Project2/FX/{0}.{0}");
	const FString SFX_PATH_FORMAT = TEXT("/Game/Project2/SFX/{0}.{0}");

	FStreamableManager* StreamableMgr;

};

