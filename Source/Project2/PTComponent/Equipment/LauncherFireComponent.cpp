#include "PTComponent/Equipment//LauncherFireComponent.h"
#include "PTGameModeBase.h"
#include "PTActor/PTProjectile.h"
#include "Engine/DamageEvents.h"
#include "GameData/PTGameDataSingleton.h"
#include "PTComponent/PTFactionComponent.h"

void ULauncherFireComponent::FireProcess(FVector SpawnPoint, float Range, int32 Damage, FString ProjectileKey)
{
	// 폰이 보고있는 시야의 시작 위치와 회전방향을 가져온다. (카메라가 붙어있는 경우 카메라 베이스로, 없는 경우는 모르겠다.)
	FVector OutLocation;
	FRotator OutRotation;
	GunOwner->GetController()->GetPlayerViewPoint(OutLocation, OutRotation);
	
	IPTGameInterface* GameMode = Cast<IPTGameInterface>(GetWorld()->GetAuthGameMode());

	APTProjectile* Projectile = GameMode->GetSpawnManager()->SpawnObject<APTProjectile>(OutRotation, SpawnPoint, true);
	Projectile->SetOwner(GunOwner);
	Projectile->OnExplosion.BindLambda([this, Damage](const TArray<FOverlapResult>& OverlapResults, FVector Location){
		ApplyDamageToEnemies(GunOwner, OverlapResults, Location, Damage);
			
		if (HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
	});
		
	Projectile->Init(*ProjectileKey);
}

void ULauncherFireComponent::ApplyDamageToEnemies(AActor* ProjectileOwner, const TArray<FOverlapResult>& OverlapResults, FVector Location, int32 Damage)
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
