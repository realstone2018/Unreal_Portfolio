#include "PTComponent/Equipment//GunFireComponent.h"
#include "Character/PTCharacterBase.h"

void UGunFireComponent::Init(APTGun* InRifle)
{
	Gun = InRifle;
	GunOwner = Gun->GetOwner();
}
