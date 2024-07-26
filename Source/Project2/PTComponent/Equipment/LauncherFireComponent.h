#pragma once

#include "CoreMinimal.h"
#include "PTComponent/Equipment/GunFireComponent.h"
#include "Engine/OverlapResult.h"
#include "LauncherFireComponent.generated.h"

UCLASS()
class PROJECT2_API ULauncherFireComponent : public UGunFireComponent
{
	GENERATED_BODY()

public:
	void FireProcess(FVector SpawnPoint, float Range, int Damage) override;
	void ApplyDamageToEnemies(AActor* GunOwner, const TArray<FOverlapResult>& Targets, FVector Location, int InDamage);
};
