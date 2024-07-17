#pragma once

#include "CoreMinimal.h"
#include "ABAI.h"
#include "PTActor/PTProjectile.h"
#include "GameFramework/GameModeBase.h"
#include "Manager/MonsterSpawnManager.h"
#include "Manager/PTObjectPoolManager.h"
#include "PTInterface/PTGameInterface.h"
#include "PTGameModeBase.generated.h"

UCLASS()
class PROJECT2_API APTGameModeBase : public AGameModeBase, public IPTGameInterface
{
	GENERATED_BODY()

public:
	APTGameModeBase();
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMonsterSpawnManager> MonsterSpawnManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPTObjectPoolManager> PoolManager;

	UPROPERTY(VisibleAnywhere, Category= "Stage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APTStructure> MainStationClass; 
	
	UPROPERTY(VisibleAnywhere, Category= "Stage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APTStructure> MainStation; 

	FORCEINLINE virtual APTStructure* GetMainStation() override { return MainStation; }
	
private:

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

#pragma region Object Pool
public:
	UFUNCTION()
	void SpawnMonster();

	UFUNCTION()
	void ReturnPoolMonster(AActor* Monster);
	
	UFUNCTION()
	virtual APTProjectile* SpawnProjectile(int ProjectileCode, FVector SpawnPosition, FRotator SpawnRotator) override;
#pragma endregion
	
};


