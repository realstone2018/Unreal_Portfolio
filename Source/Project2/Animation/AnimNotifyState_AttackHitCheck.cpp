#include "Animation/AnimNotifyState_AttackHitCheck.h"
#include "PTInterface/PTAnimationInterface.h"

void UAnimNotifyState_AttackHitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	if (MeshComp) {
		IPTAnimationInterface* AttackPawn = Cast<IPTAnimationInterface>(MeshComp->GetOwner());	
		if (AttackPawn) {
			AttackPawn->OnNotifyAttack(); 
		}
	}
}
