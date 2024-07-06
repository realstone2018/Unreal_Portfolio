#include "PTComponent/Weapon/LauncherFireComponent.h"

#include "PTProjectile.h"
#include "Engine/DamageEvents.h"
#include "PTComponent/PTFactionComponent.h"

bool ULauncherFireComponent::FireProcess(FVector SpawnPoint, float Range, int Damage)
{
	UE_LOG(LogTemp, Display, TEXT("URifleFireComponent::FireProcess"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection, SpawnPoint, Range);
	if (!bSuccess)
	{
		return false;
	}

	return false;
}

bool ULauncherFireComponent::GunTrace(FHitResult& Hit, FVector& ShotDirection, FVector SpawnPoint, float Range)
{
	UE_LOG(LogTemp, Display, TEXT("URifleFireComponent::GunTrace"));
	
	// 폰이 보고있는 시야의 시작 위치와 회전방향을 가져온다. (카메라가 붙어있는 경우 카메라 베이스로, 없는 경우는 모르겠다.)
	FVector OutLocation;
	FRotator OutRotation;
	GunOwner->GetController()->GetPlayerViewPoint(OutLocation, OutRotation);
	
	//컨트롤러를 가져와, 컨트롤러를 통해 뷰포트의 Location과 Rotation을 가져온다.
	// 폰이 보고있는 시야의 시작 위치와 회전방향을 가져온다. (카메라가 붙어있는 경우 카메라 베이스로, 없는 경우는 모르겠다.)
	FVector End = OutLocation + OutRotation.Vector() * Range;
	ShotDirection =	-OutRotation.Vector();
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(GunOwner);

	DrawDebugLine(GetWorld(), OutLocation, End, FColor::Red, true, 5.f);

	//어딘가에 부딪쳐야만 True반환
	//return GetWorld()->LineTraceSingleByChannel(Hit, OutLocation, End, ECC_GameTraceChannel1, Params);

	APTProjectile* Projectile = GetWorld()->SpawnActor<APTProjectile>(ProjectileClass, SpawnPoint, OutRotation);
	if (Projectile)
	{
		Projectile->Init(GunOwner);
		Projectile->OnHitArea.BindLambda([](AActor* ProjectileOwner, const TArray<AActor*>& Targets){
			UE_LOG(LogTemp, Display, TEXT("ULauncherFireComponent::GunTrace - OnHitArea Callback"));

			APTCharacterBase* OwnerCharacter = Cast<APTCharacterBase>(ProjectileOwner);	

			for (AActor* Target : Targets)
			{
				APTCharacterBase* TargetCharacter = Cast<APTCharacterBase>(Target);
				
				if (!TargetCharacter)
				{
					continue;
				}
				
				if (TargetCharacter->FactionComponent->IsNoneFaction() ||
					OwnerCharacter->FactionComponent->CompareFaction(TargetCharacter->FactionComponent))
				{
					continue;
				}
				
				UE_LOG(LogTemp, Display, TEXT("ULauncherFireComponent::GunTrace - Target: %s"), *Target->GetName());
				
				AController* MyOwnerInstigator = OwnerCharacter->GetInstigatorController();
				UClass* DamageTypeClass = UDamageType::StaticClass();


				
				// UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
				//
				// //TODO: 이런식으로 개별 체크하지말고 공용 파티클, 사운드 매니저를 파서 거기서 널체크하기 
				// if (HitParticles)
				// {
				// 	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
				// }
				//
				// if (HitSound)
				// {
				// 	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
				// }

				//TODO: 쉐이크는 기능 완성되고 나서 여유있을때
				// if (HitCameraShakeClass)
				// {
				// 	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
				// }
			}
		});
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile Spawn Fail"));	
	}
	
	return true;
}

