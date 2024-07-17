#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackCooldown.generated.h"

UCLASS()
class PROJECT2_API UBTDecorator_AttackCooldown : public UBTDecorator
{
	GENERATED_BODY()

	UBTDecorator_AttackCooldown();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
