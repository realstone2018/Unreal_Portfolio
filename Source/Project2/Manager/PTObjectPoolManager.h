#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interface/PTPullingObjectInterface.h"
#include "Character/PTMonster.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "PTObjectPoolManager.generated.h"

UENUM(BlueprintType)
enum class EPoolListType : uint8
{
    None = 0,
    Monster UMETA(DisplayName = "Monster"),
};

USTRUCT(BlueprintType)
struct FPoolArray
{
    GENERATED_BODY()

    FPoolArray() : ParentActor(nullptr) {}
    FPoolArray(AActor* Parent) : ParentActor(Parent) {}
    
    UPROPERTY()
    TArray<class AActor*> PoolArray;

    UPROPERTY()
    AActor* ParentActor;
};

UCLASS()
class PROJECT2_API UPTObjectPoolManager : public UObject
{
	GENERATED_BODY()

public:
    UPTObjectPoolManager();

    void Init(UWorld* World);
 
    template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
    void InitializePool(EPoolListType PoolType, TSubclassOf<T> ObjectClass, int32 InitialSize);

    template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
    T* GetPooledObject(EPoolListType PoolType, FTransform const& Trans);
    
    void ReturnPooledObject(AActor* Object);
    
    UPROPERTY()
    TMap<EPoolListType, FPoolArray> PoolMap;
        
    UPROPERTY()
    TSubclassOf<AActor> PooledObjectClass;

    UPROPERTY()
    UWorld* WorldContext;

    UPROPERTY()
    TObjectPtr<class UObjectPoolData> PoolData;
};


