#include "AI/BTTask_ScorchAttack.h"
#include "AIController.h"
#include "Character/PTMonster.h"

UBTTask_ScorchAttack::UBTTask_ScorchAttack()
{
	
}

EBTNodeResult::Type UBTTask_ScorchAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APTMonster* Monster = Cast<APTMonster>(AIController->GetPawn());
	if (Monster == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Monster->PlayAttackMontage();
	return EBTNodeResult::Succeeded;	
}
