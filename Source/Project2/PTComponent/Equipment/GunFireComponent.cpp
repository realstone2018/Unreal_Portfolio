#include "PTComponent/Equipment//GunFireComponent.h"
#include "PTProjectile.h"

void UGunFireComponent::Init(AActor* InRifle, TSubclassOf<APTProjectile> InProjectileClass)
{
	Gun = InRifle;
	GunOwner = Cast<APTCharacterBase>(Gun->GetOwner());

	ProjectileClass = InProjectileClass;
}
