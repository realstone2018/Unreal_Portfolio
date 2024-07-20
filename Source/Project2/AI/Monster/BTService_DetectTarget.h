#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectTarget.generated.h"

UCLASS()
class PROJECT2_API UBTService_DetectTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DetectTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UObject* DetectPlayer(const UWorld* World, const TArray<FOverlapResult>& OverlapResults, FVector DetectLocation, float DetectRadius);
	UObject* DetectStructure(const UWorld* World, const TArray<FOverlapResult>& OverlapResults, FVector DetectLocation, float DetectRadius);
	void UpdateBlackboardValue(UBlackboardComponent* OwnerComp, FName KeyName, UObject* NewValue);
};
