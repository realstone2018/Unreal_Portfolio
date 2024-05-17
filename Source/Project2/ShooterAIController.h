// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT2_API AShooterAIController : public AAIController
{
	GENERATED_BODY()


public:
	AShooterAIController();
	
	bool IsDead() const;

protected:
	virtual void BeginPlay() override;

private:
	//UPROPERTY(EditAnywhere)
	//float AcceptanceRadius = 200.f;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
};
