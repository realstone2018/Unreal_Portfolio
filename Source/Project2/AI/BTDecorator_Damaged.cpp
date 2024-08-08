#include "AI/BTDecorator_Damaged.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PTInterface/PTMonsterAIInterface.h"

bool UBTDecorator_Damaged::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (OwnerPawn == nullptr || OwnerPawn->GetController() == nullptr)
	{
		return false;
	}
	
	IPTAIInterface* AIPawn = Cast<IPTAIInterface>(OwnerPawn);
	if (AIPawn == nullptr)
	{
		return false;
	}
	
	return AIPawn->GetHpRatio() < 1.0f;
}
