#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Manager/MonsterSpawnManager.h"
#include "Manager/PTObjectPoolManager.h"
#include "PTGameModeBase.generated.h"

UCLASS()
class PROJECT2_API APTGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APTGameModeBase();
	virtual void BeginPlay() override;

	void PawnDead(APawn* Victim);

	UPROPERTY(EditAnywhere)
	UMonsterSpawnManager* MonsterSpawnManager;

	UPROPERTY()
	UPTObjectPoolManager* PoolManager;
	
private:

#pragma region Timer 
	void TimerStart();

	UFUNCTION()
	void TimerEnd();

	
#pragma endregion

#pragma region Result
	bool WinCondition();
	bool LoseCondition(APawn* PawnKilled);
	void StageWin();
	void StageLose();
	void EndGame();

#pragma endregion

private:
	UFUNCTION()
	void SpawnMonster(int num);
	
	UPROPERTY()
	APTMonster* Monster;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class APTMonster> MonsterClass;
	
};


