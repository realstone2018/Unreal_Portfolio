#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_InRage.generated.h"

UCLASS()
class PROJECT2_API UBTTask_InRage : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void RageAround(TArray<FOverlapResult> OverlapResults, UObject* LastAttacker);
};
