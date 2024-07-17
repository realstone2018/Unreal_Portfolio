#include "AI/Monster//BTTask_ScorchAttack.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PTInterface/PTMonsterAIInterface.h"
#include "AIController.h"
#include "ABAI.h"

EBTNodeResult::Type UBTTask_ScorchAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result =  Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IPTAIInterface* AIPawn = Cast<IPTAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 공격 애니메이션 진행, 타격 판정까지 끝나야 Attack이 끝났다가 할수 있다.
	// 공격의 종료 판정을 틱함수가 아닌, 델리게이트로
	FAICharacterAttackFinished OnAttackFinished ;
	OnAttackFinished.BindLambda(
		[&]()
		{
			float CurrentTime = GetWorld()->GetTimeSeconds();
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_LAST_ATTACK_TIME, CurrentTime);
			
			//테스크가 종료되었음을 알림
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);		
		}
	);
	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	
	//공격 진행, 공격이 진행중이니 InProgress로 설정
	float AttackCooldown = 0.f;
	AIPawn->AttackByAI(AttackCooldown);
	
	return EBTNodeResult::InProgress;
}
