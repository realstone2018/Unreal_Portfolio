#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	TObjectPtr<APawn> GunOwner;
	
public:
	virtual void Init(AActor* InRifle) PURE_VIRTUAL(UGunFireComponent::FireProcess, ;);
	virtual bool FireProcess(float Range, int Damage) PURE_VIRTUAL(UGunFireComponent::FireProcess, return false;);
	virtual bool GunTrace(FHitResult& Hit, FVector& ShotDirection, float Range) PURE_VIRTUAL(UGunFireComponent::GunTrace, return false;);

	FOnHitTracing OnHitTracing;
};
