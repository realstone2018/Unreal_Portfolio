#include "PTComponent/Equipment//LauncherFireComponent.h"
#include "PTGameModeBase.h"
#include "Manager/PTAssetManager.h"
#include "GameData/PTGameDataSingleton.h"
#include "PTActor/PTProjectile.h"

#define ENABLE_DRAW_DEBUG 0

void ULauncherFireComponent::SetProjectile(FString ProjectileKey)
{
	CurrentProjectile = *ProjectileKey;
	LoadResource();
}

void ULauncherFireComponent::LoadResource()
{
	FPTProjectileData ProjectileData = UPTGameDataSingleton::Get().GetProjectileData(CurrentProjectile);
	UPTAssetManager& AssetManager = UPTAssetManager::Get();
	AssetManager.LoadMeshAsset<UStaticMesh>(ProjectileData.StaticMesh, nullptr);
	AssetManager.LoadFXAsset<UParticleSystem>(ProjectileData.TrailEffect, nullptr);
	AssetManager.LoadFXAsset<UParticleSystem>(ProjectileData.ExplosionEffect, nullptr);
	AssetManager.LoadSFXAsset<USoundWave>(ProjectileData.ExplosionSound, nullptr);
}

void ULauncherFireComponent::FireProcess(FVector SpawnPoint, FVector ViewLocation, FRotator ViewDirection, float Range, int32 Damage)
{
	FVector End = ViewLocation + ViewDirection.Vector() * Range;
	FVector ShotDirection = End - SpawnPoint;
	FRotator ShotRot = FRotationMatrix::MakeFromX(ShotDirection).Rotator();
	
	IPTGameInterface* GameMode = Cast<IPTGameInterface>(GetWorld()->GetAuthGameMode());
	APTProjectile* Projectile = GameMode->GetSpawnManager()->SpawnObject<APTProjectile>(CurrentProjectile, ShotRot, SpawnPoint, true);
	Projectile->SetOwner(Gun->GetOwner());
	Projectile->OnExplosion.BindLambda([this, Damage](const TArray<FOverlapResult>& OverlapResults, FVector Location){
		Gun->DamageToOverlapResults(OverlapResults, Location, Damage);
			
		if (HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
	});

#if ENABLE_DRAW_DEBUG
	DrawDebugLine(GetWorld(), ViewLocation, End, FColor::Green, true, 5.f);;

	DrawDebugLine(GetWorld(), SpawnPoint, SpawnPoint + ShotRot.Vector() * Range, FColor::Red, true, 5.f);;
#endif
}
