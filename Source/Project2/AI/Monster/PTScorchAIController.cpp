#include "AI/Monster/PTScorchAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ABAI.h"
#include "PTInterface/PTMonsterAIInterface.h"

APTScorchAIController::APTScorchAIController()
{
}

void APTScorchAIController::RunAI(AActor* DefaultTarget)
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

	RunAI(nullptr);
}
