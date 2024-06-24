#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Character/PTScorch.h"
#include "Character/PTMonster.h"
#include "MonsterSpawnManager.generated.h"

UCLASS()
class PROJECT2_API UMonsterSpawnManager : public UObject
{
	GENERATED_BODY()

public:
	UMonsterSpawnManager();

	// UFUNCTION()
	// void SpawnMonster(int num);
	//
	// UPROPERTY()
	// APTMonster* Monster;
	//
	// UPROPERTY(EditAnywhere, Category = "Spawning")
	// TSubclassOf<class APTMonster> MonsterClass;
};
