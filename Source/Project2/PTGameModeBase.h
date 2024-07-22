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
	

#pragma region Timer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stage", meta = (AllowPrivateAccess = "true"))
	float StageClearTime = 300.f;
	
	void TimerStart();

	UFUNCTION()
	void TimerEnd();

	
#pragma endregion


#pragma region Result
	void PawnDead(APawn* Victim);

	bool WinCondition();
	bool LoseCondition(APawn* PawnKilled);
	void StageWin();
	void StageLose();
	void EndGame();

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

#pragma endregion 
	
};


