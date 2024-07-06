#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/PTCharacterBase.h"
#include "GunFireComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FOnHitTracing, FHitResult /*hit*/, FVector /*ShotDirection*/)

UCLASS(Abstract)
class PROJECT2_API UGunFireComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> Gun;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APTCharacterBase> GunOwner;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APTProjectile> ProjectileClass;

public:
	virtual void Init(AActor* InRifle, TSubclassOf<APTProjectile> InProjectileClass);
	virtual bool FireProcess(FVector SpawnPoint, float Range, int Damage) PURE_VIRTUAL(UGunFireComponent::FireProcess, return false;);
	virtual bool GunTrace(FHitResult& Hit, FVector& ShotDirection, FVector SpawnPoint, float Range) PURE_VIRTUAL(UGunFireComponent::GunTrace, return false;);

	FOnHitTracing OnHitTracing;
};
