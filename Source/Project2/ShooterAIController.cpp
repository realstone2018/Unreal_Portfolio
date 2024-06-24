// Fill		out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

AShooterAIController::AShooterAIController()
{
}

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		
		//BB에 PlayerLocation 키가 만들어져 있어야 한다. 
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

bool AShooterAIController::IsDead()	 const
{
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead();
	}

	//캐릭터가 사망시 컨트롤러가 분리되니 GetPawn()이 null일수 있음, 이 경우 당연히 true
	return true;
}
	