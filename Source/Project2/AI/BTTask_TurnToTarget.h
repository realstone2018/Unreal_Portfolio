#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_TurnToTarget.generated.h"

UCLASS()
class PROJECT2_API UBTTask_TurnToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_TurnToTarget();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
