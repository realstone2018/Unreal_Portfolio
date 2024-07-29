#pragma once

#include "CoreMinimal.h"
#include "PTComponent/Equipment/GunFireComponent.h"
#include "RifleFireComponent.generated.h"

UCLASS()
class PROJECT2_API URifleFireComponent : public UGunFireComponent
{
	GENERATED_BODY()

public:
	void FireProcess(FVector SpawnPoint, float Range, int32 Damage) override;
	uint8 GunTrace(FHitResult& HitResult, FVector& ShotDirection, FVector SpawnPoint, float Range);
};
