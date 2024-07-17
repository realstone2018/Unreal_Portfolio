#include "AI/BTDecorator_AttackInRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PTInterface/PTMonsterAIInterface.h"
#include "AIController.h"
#include "VectorUtil.h"
#include "Components/ShapeComponent.h"
#include "ABAI.h"

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}
	
	IPTAIInterface* AIPawn = Cast<IPTAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(	GetSelectedBlackboardKey()));
	if (nullptr == Target)
	{
		return false;
	}
	
	// auto* Collision = Target->GetComponentByClass<UShapeComponent>();
	// FVector Direction = (Target->GetActorLocation() - ControllingPawn->GetActorLocation()).GetSafeNormal();
	//
	// FVector Center = Collision->GetComponentLocation(); 
	// float Radius = Center + 

	//FVector TargetPos = Target->GetActorLocation() + (Direction * Collision->)

	//UE::Geometry::VectorUtil
	
	//FVector::Dist2D(ControllingPawn->GetActorLocation(), Target->GetActorLocation());
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRange = AIPawn->GetAIAttackRange();
	bResult = (DistanceToTarget <= AttackRange);
	return bResult;
}
