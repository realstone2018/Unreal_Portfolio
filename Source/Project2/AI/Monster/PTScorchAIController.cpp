// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Monster/PTScorchAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ABAI.h"

APTScorchAIController::APTScorchAIController()
{
}

void APTScorchAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	//블랙보드 구동
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		////12-2 전처리를 키값으로 홈 포지션 세팅
		//Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());
		
		//행동트리 구동
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void APTScorchAIController::StopAI()
{
	//실행중인 행동트리는 AIConroller.BrainComponent에 저장된다. 이를 캐스팅
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

// <!> Shooter는 전부 배치해놓고 Spawn이 없어서
// 이게 없었음 
void APTScorchAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
