#include "AI/Monster/BTTask_InRage.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Character/PTMonster.h"
#include "Engine/OverlapResult.h"
#include "Physics/PTCollision.h"
#include "AI/ABAI.h"

EBTNodeResult::Type UBTTask_InRage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UE_LOG(LogTemp, Display, TEXT("UBTTask_InRage::ExecuteTask() - Owner: %s"), *OwnerComp.GetAIOwner()->GetName());

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (OwnerPawn == nullptr || OwnerPawn->GetController() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UWorld* World = OwnerPawn->GetWorld();
	FVector OwnerLocation = OwnerPawn->GetActorLocation();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, OwnerPawn);
	uint8 bResult = World->OverlapMultiByChannel(
		OverlapResults,
		OwnerLocation,
		FQuat::Identity,
		CCHANNEL_PTBULLET,
		FCollisionShape::MakeSphere(500.f),
		CollisionQueryParam
	);

	if (IPTAIInterface* AIMonster = Cast<IPTAIInterface>(OwnerPawn))
	{
		UObject* LastAttacker = AIMonster->GetLastAttacker();
		if (LastAttacker)
		{
			UE_LOG(LogTemp, Display, TEXT("UBTTask_InRage::ExecuteTask() - Monster: %s  LastAttacker: %s"), *OwnerPawn->GetName(), *LastAttacker->GetName());

			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, LastAttacker);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_INRAGE, true);
			RageAround(OverlapResults, LastAttacker);
		}
	}
	
	return EBTNodeResult::InProgress;
}

void UBTTask_InRage::RageAround(TArray<FOverlapResult> OverlapResults, UObject* LastAttacker)
{
	for (auto const& OverlapResult : OverlapResults)
	{
		if (IPTAIInterface* OverlapAIMonster = Cast<IPTAIInterface>(OverlapResult.GetActor()))
		{
			UE_LOG(LogTemp, Display, TEXT("UBTTask_InRage::ExecuteTask() - Monster: %s  LastAttacker: %s"), *OverlapResult.GetActor()->GetName(), *LastAttacker->GetName());
			
			AAIController* OverlapAIController = Cast<AAIController>(OverlapResult.GetActor()->GetInstigatorController());
			if (OverlapAIController)
			{
				UBlackboardComponent* OverlapBlackboard = OverlapAIController->GetBlackboardComponent();
				if (OverlapBlackboard)
				{
					OverlapBlackboard->SetValueAsObject(BBKEY_TARGET, LastAttacker);
					OverlapBlackboard->SetValueAsBool(BBKEY_INRAGE, true);
				}
			}			
		}
	}		
}
