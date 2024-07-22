#pragma once

#include "CoreMinimal.h"
#include "PTActor/PTStructure.h"
#include "UObject/Interface.h"
#include "GameData/ObjectPoolData.h"
#include "Manager/PTSpawnManager.h"
#include "Manager/PTObjectPoolManager.h"
#include "PTGameInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPTGameInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT2_API IPTGameInterface
{
	GENERATED_BODY()

public:
	virtual APTStructure* GetMainStation() = 0;

	virtual UPTObjectPoolManager* GetObjectPoolManager() = 0;
	virtual UPTSpawnManager* GetSpawnManager() = 0;

};
