#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PTMonsterAIController.generated.h"

UCLASS()
class PROJECT2_API APTMonsterAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	void RunAI(AActor* DefaultTarget);
	void StopAI();

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<class UBlackboardData> BBAsset;
};
