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

public:
	virtual void Init(AActor* InRifle, TSubclassOf<class APTProjectile> InProjectileClass);
	virtual void FireProcess(FVector SpawnPoint, float Range, int32 Damage) PURE_VIRTUAL(UGunFireComponent::FireProcess, ;);

	FOnHitTracing OnHitTracing;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> Gun;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APTCharacterBase> GunOwner;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APTProjectile> ProjectileClass;
};
