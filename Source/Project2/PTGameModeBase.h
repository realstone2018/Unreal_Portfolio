#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PTInterface/PTGameInterface.h"
#include "PTGameModeBase.generated.h"

UCLASS()
class PROJECT2_API APTGameModeBase : public AGameModeBase, public IPTGameInterface
{
	GENERATED_BODY()

public:
	APTGameModeBase();
	
	virtual void BeginPlay() override;

	virtual void StartPlay() override;

#pragma region Flow
public:
	void GameStart();
	void GameClear();
	void GameOver();

	virtual void OnPlayerDead() override;
#pragma endregion


#pragma region Timer
public:
	void StageTimerStart();
	void MonsterWaveTimerStart();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	float GetStageRemainTime();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stage", meta = (AllowPrivateAccess = "true"))
	float StageClearTime = 100.f;

	UPROPERTY()
	FTimerHandle StageTimerHandle;
	
#pragma endregion


#pragma region Object Pool, Spawn
public:
	FORCEINLINE UPTObjectPoolManager* GetObjectPoolManager() override { return PoolManager; }
	FORCEINLINE UPTSpawnManager* GetSpawnManager() override { return SpawnManager; }
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPTObjectPoolManager> PoolManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPTSpawnManager> SpawnManager;

#pragma endregion


#pragma region Structure
public:
	FORCEINLINE virtual APTStructure* GetMainStation() override { return MainStation; }

private:
	UPROPERTY(VisibleAnywhere, Category= "Stage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APTStructure> MainStationClass; 
	
	UPROPERTY(VisibleAnywhere, Category= "Stage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APTStructure> MainStation; 

	FORCEINLINE virtual void OnMainStationDestruct() override { GameOver(); }
#pragma endregion 
	
};


