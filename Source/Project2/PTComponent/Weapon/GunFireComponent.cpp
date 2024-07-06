#include "PTComponent/Weapon/GunFireComponent.h"
#include "PTProjectile.h"

void UGunFireComponent::Init(AActor* InRifle, TSubclassOf<APTProjectile> InProjectileClass)
{
	UE_LOG(LogTemp, Display, TEXT("UGunFireComponent::Init"));

	Gun = InRifle;
	GunOwner = Cast<APTCharacterBase>(Gun->GetOwner());

	ProjectileClass = InProjectileClass;
}
