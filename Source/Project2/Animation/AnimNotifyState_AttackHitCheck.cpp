#include "Animation/AnimNotifyState_AttackHitCheck.h"
#include "PTInterface/PTAnimationAttackInterface.h"

void UAnimNotifyState_AttackHitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	if (MeshComp) {
		//노타파이의 대상이 인터페이스를 상속받았는지 체크
		IPTAnimationAttackInterface* AttackPawn = Cast<IPTAnimationAttackInterface>(MeshComp->GetOwner());	
		if (AttackPawn) {
			AttackPawn->OnNotifyAttack(); 
		}
	}
}
