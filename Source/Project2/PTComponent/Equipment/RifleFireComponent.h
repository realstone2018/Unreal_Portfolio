#pragma once

#include "CoreMinimal.h"
#include "PTComponent/Equipment/GunFireComponent.h"
#include "RifleFireComponent.generated.h"

UCLASS()
class PROJECT2_API URifleFireComponent : public UGunFireComponent
{
	GENERATED_BODY()

public:
	void FireProcess(FVector SpawnPoint, FVector ViewLocation, FRotator ViewDirection, float Range, int32 Damage) override;

private:
	uint8 FireLineTracing(FHitResult& HitResult, FVector SpawnPoint, FVector End);
};
