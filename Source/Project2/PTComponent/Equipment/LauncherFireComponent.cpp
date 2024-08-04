#include "PTComponent/Equipment//LauncherFireComponent.h"
#include "PTGameModeBase.h"
#include "PTActor/PTProjectile.h"
#include "Engine/DamageEvents.h"
#include "GameData/PTGameDataSingleton.h"
#include "Manager/PTAssetManager.h"
#include "PTComponent/PTFactionComponent.h"

void ULauncherFireComponent::Init(AActor* InRifle)
{
	Super::Init(InRifle);
	
	FPTProjectileData ProjectileData = UPTGameDataSingleton::Get().GetProjectileData(CurrentProjectile);
	UPTAssetManager& AssetManager = UPTAssetManager::Get();
	AssetManager.LoadMeshAsset<UStaticMesh>(ProjectileData.StaticMesh, nullptr);
	AssetManager.LoadFXAsset<UParticleSystem>(ProjectileData.TrailEffect, nullptr);
	AssetManager.LoadFXAsset<UParticleSystem>(ProjectileData.ExplosionEffect, nullptr);
	AssetManager.LoadSFXAsset<USoundWave>(ProjectileData.ExplosionSound, nullptr);
}

void ULauncherFireComponent::FireProcess(FVector SpawnPoint, FRotator ShotDirection, float Range, int32 Damage)
{
	IPTGameInterface* GameMode = Cast<IPTGameInterface>(GetWorld()->GetAuthGameMode());

	APTProjectile* Projectile = GameMode->GetSpawnManager()->SpawnObject<APTProjectile>(ShotDirection, SpawnPoint, true);
	Projectile->SetOwner(GunOwner);
	Projectile->OnExplosion.BindLambda([this, Damage](const TArray<FOverlapResult>& OverlapResults, FVector Location){
		TakeDamageToOverlapResults(GunOwner, OverlapResults, Location, Damage);
			
		if (HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
	});
		
	Projectile->SetData(CurrentProjectile);
}

void ULauncherFireComponent::TakeDamageToOverlapResults(AActor* ProjectileOwner, const TArray<FOverlapResult>& OverlapResults, FVector Location, int32 Damage)
{
	APTCharacterBase* OwnerCharacter = Cast<APTCharacterBase>(ProjectileOwner);	

	for (FOverlapResult Target : OverlapResults)
	{
		APTCharacterBase* TargetCharacter = Cast<APTCharacterBase>(Target.GetActor());
				
		if (!TargetCharacter)
		{
			continue;
		}

		UPTFactionComponent* TargetFaction = TargetCharacter->GetFactionComponent();
		if (TargetFaction->IsNoneFaction() || OwnerCharacter->GetFactionComponent()->CompareFaction(TargetFaction))
		{
			continue;
		}
		
		FRadialDamageEvent RadialDamageEvent;
		RadialDamageEvent.Params.BaseDamage = Damage;
		RadialDamageEvent.Params.MinimumDamage = Damage;
		RadialDamageEvent.Origin = Location;
		TargetCharacter->TakeDamage(Damage, RadialDamageEvent, OwnerCharacter->GetController(), Gun);
	}
}
