#include "Animation/AnimNotifyState_AttackHitCheck.h"
#include "PTInterface/PTAnimationAttackInterface.h"

void UAnimNotifyState_AttackHitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	if (MeshComp) {
		IPTAnimationAttackInterface* AttackPawn = Cast<IPTAnimationAttackInterface>(MeshComp->GetOwner());	
		if (AttackPawn) {
			AttackPawn->OnNotifyAttack(); 
		}
	}
}
