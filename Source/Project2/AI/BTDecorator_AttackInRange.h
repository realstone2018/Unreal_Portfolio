#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_AttackInRange.generated.h"

UCLASS()
class PROJECT2_API UBTDecorator_AttackInRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
