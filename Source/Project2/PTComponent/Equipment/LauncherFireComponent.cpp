#include "PTComponent/Equipment//LauncherFireComponent.h"
#include "PTGameModeBase.h"
#include "Manager/PTAssetManager.h"
#include "GameData/PTGameDataSingleton.h"
#include "PTActor/PTProjectile.h"

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

void ULauncherFireComponent::FireProcess(FVector SpawnPoint, FVector ViewLocation, FRotator ShotDirection, float Range, int32 Damage)
{
	IPTGameInterface* GameMode = Cast<IPTGameInterface>(GetWorld()->GetAuthGameMode());
	APTProjectile* Projectile = GameMode->GetSpawnManager()->SpawnObject<APTProjectile>(CurrentProjectile, ShotDirection, SpawnPoint, true);
	Projectile->SetOwner(Gun->GetOwner());
	Projectile->OnExplosion.BindLambda([this, Damage](const TArray<FOverlapResult>& OverlapResults, FVector Location){
		Gun->DamageToOverlapResults(OverlapResults, Location, Damage);
			
		if (HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
	});
}
