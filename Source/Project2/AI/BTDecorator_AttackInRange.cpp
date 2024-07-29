#include "AI/BTDecorator_AttackInRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PTInterface/PTMonsterAIInterface.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Util/PTVectorUtil.h"

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (OwnerPawn == nullptr)
	{
		return false;
	}

	IPTAIInterface* OwnerInterface = Cast<IPTAIInterface>(OwnerPawn);
	if (nullptr == OwnerInterface)
	{
		return false;
	}

	UCapsuleComponent* OwnerPawnCollision = OwnerPawn->GetComponentByClass<UCapsuleComponent>();
	if (OwnerPawnCollision == nullptr)
	{
		return false;
	}
	
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (Target == nullptr)
	{
		return false;
	}
	
	UShapeComponent* TargetCollision = Target->GetComponentByClass<UShapeComponent>();
	if (TargetCollision == nullptr)
	{
		return false;
	}

	float AttackRange = OwnerInterface->GetAIAttackRange();
	FVector3d OwnerLocation = OwnerPawn->GetActorLocation();
	FVector3d TargetLocation = Target->GetActorLocation();

	if (UCapsuleComponent* TargetCapsule = Cast<UCapsuleComponent>(TargetCollision))
	{
		float Range = AttackRange + TargetCapsule->GetScaledCapsuleRadius();
		bResult = PTVectorUtil::CollisionToCircle<double>(OwnerLocation, TargetLocation, Range);
	}
	else if (UBoxComponent* TargetBox = Cast<UBoxComponent>(TargetCollision))
	{
		//<!!!!!> Extent는 전체 넓이, 높이가 아닌 Radius 개념 
		float HalfWidth = TargetBox->GetScaledBoxExtent().X;
		float HalfHeight = TargetBox->GetScaledBoxExtent().Y;
		float boxRaidus = FMath::Sqrt((HalfWidth * HalfWidth) + (HalfHeight * HalfHeight));
		
		float Range = AttackRange + boxRaidus;
		uint8 CheckCircumcircle = PTVectorUtil::CheckInRange<double>(OwnerLocation, TargetLocation, Range);

		if (CheckCircumcircle == false)
		{
			return false;
		}
		
		float BoxRotationYaw = TargetBox->GetComponentRotation().Yaw;
		bResult = PTVectorUtil::CollisionToBox(OwnerLocation, AttackRange, TargetLocation, HalfWidth, HalfHeight, BoxRotationYaw);
	}
	
	return bResult;
}
