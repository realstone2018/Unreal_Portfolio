#include "AI/BTService_SetDashDestination.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_SetDashDestination::UBTService_SetDashDestination()
{
	NodeName = "Set Dash Destination";
	Interval = 1.0f;
}

void UBTService_SetDashDestination::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}


	FVector Direction = ControllingPawn->GetActorForwardVector();
	FVector Destination = ControllingPawn->GetActorLocation() + (Direction * 500.f); 
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Destination);
}
