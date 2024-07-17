#pragma once

#include "CoreMinimal.h"
#include "PTActor/PTStructure.h"
#include "UObject/Interface.h"
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
	virtual APTProjectile* SpawnProjectile(int ProjectileCode, FVector SpawnPosition, FRotator SpawnRotator) = 0;

};
