#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_SetDashDestination.generated.h"

UCLASS()
class PROJECT2_API UBTService_SetDashDestination : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UBTService_SetDashDestination();

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
