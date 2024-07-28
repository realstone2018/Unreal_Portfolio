#include "AI/Monster/PTMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ABAI.h"

void APTMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI(nullptr);
}

void APTMonsterAIController::RunAI(AActor* DefaultTarget)
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		if (DefaultTarget)
		{
			Blackboard->SetValueAsObject(BBKEY_TARGET_MAINSTATION, DefaultTarget);
		}
		
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void APTMonsterAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}