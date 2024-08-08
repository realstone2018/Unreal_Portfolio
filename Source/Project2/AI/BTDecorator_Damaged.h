#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_Damaged.generated.h"

UCLASS()
class PROJECT2_API UBTDecorator_Damaged : public UBTDecorator
{
	GENERATED_BODY()
		
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
