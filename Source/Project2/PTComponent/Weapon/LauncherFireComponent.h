#pragma once

#include "CoreMinimal.h"
#include "PTComponent/Weapon/GunFireComponent.h"
#include "LauncherFireComponent.generated.h"

UCLASS()
class PROJECT2_API ULauncherFireComponent : public UGunFireComponent
{
	GENERATED_BODY()

public:
	virtual bool FireProcess(FVector SpawnPoint, float Range, int Damage) override;
	virtual bool GunTrace(FHitResult& Hit, FVector& ShotDirection, FVector SpawnPoint, float Range) override;
};
