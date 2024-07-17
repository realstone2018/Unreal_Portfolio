#include "PTComponent/Equipment//LauncherFireComponent.h"
#include "PTGameModeBase.h"
#include "PTActor/PTProjectile.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "PTComponent/PTFactionComponent.h"

void ULauncherFireComponent::FireProcess(FVector SpawnPoint, float Range, int Damage)
{	
	// 폰이 보고있는 시야의 시작 위치와 회전방향을 가져온다. (카메라가 붙어있는 경우 카메라 베이스로, 없는 경우는 모르겠다.)
	FVector OutLocation;
	FRotator OutRotation;
	GunOwner->GetController()->GetPlayerViewPoint(OutLocation, OutRotation);
	
	IPTGameInterface* GameMode = Cast<IPTGameInterface>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		APTProjectile* Projectile2 = GameMode->SpawnProjectile(0, SpawnPoint, OutRotation);
		ensure(Projectile2);

		Projectile2->Init(GunOwner);
		Projectile2->OnExplosion.BindLambda([this, Damage](AActor* ProjectileOwner, const TArray<FOverlapResult>& OverlapResults){
			ApplyDamageToEnemies(ProjectileOwner, OverlapResults, Damage);
		});
	}
}

void ULauncherFireComponent::ApplyDamageToEnemies(AActor* ProjectileOwner, const TArray<FOverlapResult>& OverlapResults, int Damage)
{
	APTCharacterBase* OwnerCharacter = Cast<APTCharacterBase>(ProjectileOwner);	

	for (FOverlapResult Target : OverlapResults)
	{
		APTCharacterBase* TargetCharacter = Cast<APTCharacterBase>(Target.GetActor());
				
		if (!TargetCharacter)
		{
			continue;
		}
				
		if (TargetCharacter->FactionComponent->IsNoneFaction() ||
			OwnerCharacter->FactionComponent->CompareFaction(TargetCharacter->FactionComponent))
		{
			continue;
		}
		
		UClass* DamageTypeClass = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(TargetCharacter, Damage, OwnerCharacter->GetController(), Gun, DamageTypeClass);
	}
}
