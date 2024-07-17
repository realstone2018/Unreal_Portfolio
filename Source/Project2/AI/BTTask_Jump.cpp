#include "AI/BTTask_Jump.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "PTInterface/PTMonsterAIInterface.h"

EBTNodeResult::Type UBTTask_Jump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result =  Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IPTAIInterface* AIPawn = Cast<IPTAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	AIPawn->JumpByAI();
	return EBTNodeResult::InProgress;
}
