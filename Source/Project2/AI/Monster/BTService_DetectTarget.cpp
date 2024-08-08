#include "AI/Monster/BTService_DetectTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "PTInterface/PTMonsterAIInterface.h"
#include "Physics/PTCollision.h"
#include "Character/PTPlayerCharacter.h"
#include "PTActor/PTStructure.h"
#include "AIController.h"
#include "ABAI.h"

#define ENABLE_DRAW_DEBUG 0

UBTService_DetectTarget::UBTService_DetectTarget()
{
	NodeName = TEXT("DetectTarget");
	Interval = 1.0f;

	UE_LOG(LogTemp, Display, TEXT("UBTService_DetectTarget::UBTService_DetectTarget()"));
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (OwnerPawn == nullptr || OwnerPawn->GetController() == nullptr)
	{
		return;
	}
	
	IPTAIInterface* AIPawn = Cast<IPTAIInterface>(OwnerPawn);
	if (AIPawn == nullptr)
	{
		return;
	}

	UWorld* World = OwnerPawn->GetWorld();
	FVector OwnerLocation = OwnerPawn->GetActorLocation();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, OwnerPawn);
	uint8 bResult = World->OverlapMultiByChannel(
		OverlapResults,
		OwnerLocation,
		FQuat::Identity,
		CCHANNEL_PTMONSTER_MELEE,
		FCollisionShape::MakeSphere(AIPawn->GetAIDetectWallRange()),
		CollisionQueryParam
	);

	UObject* TargetPlayer = nullptr;
	UObject* TargetStructure = nullptr;
	
	if (bResult)
	{
		TargetPlayer = DetectPlayer(OverlapResults, OwnerLocation, AIPawn->GetAIDetectPlayerRange());
		TargetStructure = DetectStructure(OverlapResults, OwnerLocation, AIPawn->GetAIDetectWallRange());
	}
	
	bool bInRage = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_INRAGE);
	if (bInRage == false)
	{
		UpdateBlackboardValue(OwnerComp.GetBlackboardComponent(), BBKEY_TARGET, TargetPlayer);
	}
	
	UpdateBlackboardValue(OwnerComp.GetBlackboardComponent(), BBKEY_TARGET_WALL, TargetStructure);
}

UObject* UBTService_DetectTarget::DetectPlayer(const TArray<FOverlapResult>& OverlapResults, FVector DetectLocation, float DetectRadius)
{
	AActor* result = nullptr;
	
	for (auto const& OverlapResult : OverlapResults)
	{
		if (APTPlayerCharacter* TargetPlayer = Cast<APTPlayerCharacter>(OverlapResult.GetActor()))
		{
			if (TargetPlayer->IsDead())
			{
				continue;
			}
			
			FVector TargetPlayerLocation = TargetPlayer->GetActorLocation();
			float Distance = FVector::Dist2D(DetectLocation, TargetPlayerLocation);
			if (Distance <= DetectRadius)
			{
				result = TargetPlayer;
				break;
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	if (result)
	{
		DrawDebugSphere(World, DetectLocation, DetectRadius, 16, FColor::Green, false, 0.2f);
		DrawDebugPoint(World, result->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
		DrawDebugLine(World, DetectLocation, result->GetActorLocation(), FColor::Green, false, 0.27f);
	}
	else
	{
		DrawDebugSphere(World, DetectLocation, DetectRadius, 16, FColor::Red, false, 0.2f);
	}
#endif
	
	return result;
}

UObject* UBTService_DetectTarget::DetectStructure(const TArray<FOverlapResult>& OverlapResults, FVector DetectLocation, float DetectRadius)
{
	AActor* TargetStructure = nullptr;
	int32 MinDistance = MAX_int32;

	for (auto const& OverlapResult : OverlapResults)
	{
		if (APTStructure* Structure = Cast<APTStructure>(OverlapResult.GetActor()))
		{
			if (Structure->GetbIsMainStation())
			{
				continue;	
			}
			
			float Distance = FVector::Dist2D(DetectLocation, Structure->GetActorLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				TargetStructure = Structure;
			}	
		}
	}

#if ENABLE_DRAW_DEBUG
	if (TargetStructure)
	{
		DrawDebugSphere(World, DetectLocation, DetectRadius, 16, FColor::Green, false, 0.2f);
		DrawDebugPoint(World, TargetStructure->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
		DrawDebugLine(World, DetectLocation, TargetStructure->GetActorLocation(), FColor::Green, false, 0.27f);
	}
	else
	{
		DrawDebugSphere(World, DetectLocation, DetectRadius + 100.f, 16, FColor::Red, false, 0.2f);
	}
#endif
	
	return TargetStructure;
}

void UBTService_DetectTarget::UpdateBlackboardValue(UBlackboardComponent* BlackboardComp, FName KeyName, UObject* NewValue)
{
	UObject* currentValue = BlackboardComp->GetValueAsObject(KeyName);
	if (currentValue != NewValue)
	{
		BlackboardComp->SetValueAsObject(KeyName, NewValue);
	}
}
