#pragma once

#include "CoreMinimal.h"
#include "PTComponent/Weapon/GunFireComponent.h"
#include "RifleFireComponent.generated.h"

UCLASS()
class PROJECT2_API URifleFireComponent : public UGunFireComponent
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* InRifle) override;
	
	virtual bool FireProcess(float Range, int Damage) override;
	virtual bool GunTrace(FHitResult& Hit, FVector& ShotDirection, float Range) override;
};
