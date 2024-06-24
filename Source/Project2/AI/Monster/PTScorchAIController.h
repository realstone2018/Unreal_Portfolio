// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PTScorchAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT2_API APTScorchAIController : public AAIController
{
	GENERATED_BODY()

public:
	APTScorchAIController();

	void RunAI();
	void StopAI();

protected:
	//컨트롤러가 폰에 빙의될때 호출된다.
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<class UBlackboardData> BBAsset;
};
