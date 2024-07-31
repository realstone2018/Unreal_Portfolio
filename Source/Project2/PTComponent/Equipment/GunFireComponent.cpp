#include "PTComponent/Equipment//GunFireComponent.h"
#include "Character/PTCharacterBase.h"


void UGunFireComponent::Init(AActor* InRifle)
{
	Gun = InRifle;
	GunOwner = Cast<APTCharacterBase>(Gun->GetOwner());
}
