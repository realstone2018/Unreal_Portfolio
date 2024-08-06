#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/PTGun.h"
#include "GunFireComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FOnHitTracing, FHitResult /*hit*/, FVector /*ShotDirection*/)

UCLASS(Abstract)
class PROJECT2_API UGunFireComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void Init(APTGun* InRifle) { Gun = InRifle; }

	virtual void FireProcess(FVector SpawnPoint, FRotator ShotDirection, float Range, int32 Damage) PURE_VIRTUAL(UGunFireComponent::FireProcess, ;);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APTGun> Gun;
};
