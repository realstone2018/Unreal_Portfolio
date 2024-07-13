#pragma once

#include "CoreMinimal.h"
#include "PTProjectile.h"
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
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMonsterSpawnManager> MonsterSpawnManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPTObjectPoolManager> PoolManager;

private:

#pragma region Timer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stage", meta=(AllowPrivateAccess))
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
	APTProjectile* SpawnProjectile(int ProjectileCode, FVector SpawnPosition, FRotator SpawnRotator);
#pragma endregion
	
};


