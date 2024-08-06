#pragma once

#include "CoreMinimal.h"
#include "PTComponent/Equipment/GunFireComponent.h"
#include "LauncherFireComponent.generated.h"

UCLASS()
class PROJECT2_API ULauncherFireComponent : public UGunFireComponent
{
	GENERATED_BODY()

public:
	virtual void Init(APTGun* InRifle) override;
	
	void SetProjectile(FString ProjectileKey) { CurrentProjectile = *ProjectileKey; }
	void FireProcess(FVector SpawnPoint, FRotator ShotDirection, float Range, int32 Damage) override;

private:
	void LoadResource();
	
	UPROPERTY(EditAnywhere, Category = "EffectAndSound")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;

	UPROPERTY()
	FName CurrentProjectile; 
};
