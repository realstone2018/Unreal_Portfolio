#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PTInterface/PTPullingObjectInterface.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "PTObjectPoolManager.generated.h"

USTRUCT(BlueprintType)
struct FPoolArray
{
    GENERATED_BODY()

    FPoolArray(){ }

    UPROPERTY()
    TArray<AActor*> ActiveArray;

    UPROPERTY()
    TArray<AActor*> DeactiveArray;
};

UCLASS()
class PROJECT2_API UPTObjectPoolManager : public UObject
{
	GENERATED_BODY()

public:
    UPTObjectPoolManager();

    void Init(UWorld* World);
 
    template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
    void SetUpPool(EPoolType PoolType, /*TSubclassOf<T> ObjectClass,*/ int32 SetUpSize = 0);

    template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
    T* GetPooledObject(FTransform const& Transm, FName DataKey);

    template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
    void ReturnPooledObject(AActor* Object);

private:
    UPROPERTY()
    UWorld* WorldContext;

    UPROPERTY()
    TObjectPtr<class UObjectPoolData> PoolData;

    UPROPERTY()
    TMap<EPoolType, FPoolArray> PoolMap;
};


