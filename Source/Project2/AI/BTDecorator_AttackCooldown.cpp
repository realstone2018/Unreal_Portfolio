#include "AI/BTDecorator_AttackCooldown.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PTInterface/PTMonsterAIInterface.h"
#include "AIController.h"
#include "ABAI.h"

UBTDecorator_AttackCooldown::UBTDecorator_AttackCooldown()
{
	NodeName = "AttackCooldown";
}

bool UBTDecorator_AttackCooldown::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return false;
	}
	
	float LastAttackTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_LAST_ATTACK_TIME);
	float CurrentTime = World->GetTimeSeconds();
	
	bResult = CurrentTime >= LastAttackTime + AIPawn->GetAIAttackCooldown();

	UE_LOG(LogTemp, Display, TEXT("bResult[%s] - LastAttackTime[%f] + AIPawn->GetAIAttackCooldown()[%f] <= CurrentTime[%f]"),
		bResult ? TEXT("true") : TEXT("false"), LastAttackTime, AIPawn->GetAIAttackCooldown(), CurrentTime);

	
	return bResult;
}
