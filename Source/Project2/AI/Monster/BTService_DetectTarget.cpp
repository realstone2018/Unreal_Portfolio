#include "AI/Monster/BTService_DetectTarget.h"

#include "ABAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "PTInterface/PTMonsterAIInterface.h"
#include "Physics/PTCollision.h"
#include "DrawDebugHelpers.h"

UBTService_DetectTarget::UBTService_DetectTarget()
{
	NodeName = TEXT("DetectTarget");	//노드 이름 지정
	Interval = 1.0f;	//1초 단위 반복 수행
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr || ControllingPawn->GetController() == nullptr)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IPTAIInterface* AIPawn = Cast<IPTAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	float DetectWallRadius = AIPawn->GetAIDetectWallRange();
	float DetectPlayerRadius = AIPawn->GetAIDetectPlayerRange();
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_PTMONSTER_MELEE,
		FCollisionShape::MakeSphere(DetectWallRadius),
		CollisionQueryParam
		);

	bool DetectPlayer = false;
	bool DetectWall = false;
	
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				float Distance = FVector::Dist2D(Center, Pawn->GetActorLocation());

				if (Distance <= DetectPlayerRadius)
				{
					DetectPlayer = true;
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				
					DrawDebugSphere(World, Center, DetectPlayerRadius, 16, FColor::Green, false, 0.2f);

					DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
					DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				}
			}
			
			// Detect Most Min Distance Wall




			
		}
	}


	
	if (!DetectPlayer)
	{
		//폰은 감지했지만 플레이어와 충돌하지 않은 경우
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
		DrawDebugSphere(World, Center, DetectPlayerRadius, 16, FColor::Red, false, 0.2f);
	}

	if (!DetectWall)
	{
		//폰은 감지했지만 플레이어와 충돌하지 않은 경우
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET_WALL, nullptr);
		DrawDebugSphere(World, Center, DetectPlayerRadius + 100.f, 16, FColor::Orange, false, 0.2f);
	}
}
