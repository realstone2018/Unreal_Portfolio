#include "PTGun.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/PTAssetManager.h"
#include "PTComponent/Equipment/RifleFireComponent.h"
#include "PTComponent/Equipment/LauncherFireComponent.h"
#include "Character/PTCharacterBase.h"
#include "GameData/PTGameDataSingleton.h"
#include "GameData/PTGunData.h"
#include "PTInterface/PTFactionInterface.h"

APTGun::APTGun()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

	RecoilAmount = 1.0f;
	RecoilResetSpeed = 5.0f;
	FireCount = 0;
	bIsFiring = false;
}

AController* APTGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}
	
	return OwnerPawn->GetController();
}

void APTGun::Init(const FString GunDataKey)
{
	GunData = UPTGameDataSingleton::Get().GetGunData(*GunDataKey);

	MaxAmmo = GunData.MaxAmmo;
	CurrentAmmo = MaxAmmo;

	switch (GunData.GunType)
	{
		case EGunType::Rifle:
		{
			URifleFireComponent* RifleFireComponent = NewObject<URifleFireComponent>(this, URifleFireComponent::StaticClass(), TEXT("RifleFireComponent"));
			RifleFireComponent->OnHitTracing.BindUObject(this, &APTGun::PlayImpactEffectAndSound);
			GunFireComponent = RifleFireComponent;
			break;
		}

		case EGunType::Launcher:
		{
			ULauncherFireComponent* LauncherFireComponent = NewObject<ULauncherFireComponent>(this, ULauncherFireComponent::StaticClass(), TEXT("LauncherFireComponent"));
			LauncherFireComponent->SetProjectile(GunData.ProjectileName);
			GunFireComponent = LauncherFireComponent;
			break;
		}

		case EGunType::Emitter:
			break;
	}
	
	GunFireComponent->RegisterComponent();	// 엔진에 생성한 컴포넌트를 인식
	GunFireComponent->Init(this);
}

void APTGun::PullTrigger()
{
	if (CurrentAmmo <= 0)
	{
		return;
	}
	
	if (!bIsFiring)
	{
		bIsFiring = true;
		FireCount = 0;
		
		Fire();
		
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APTGun::Fire, GunData.FireRate, true);
	}
}

void APTGun::StopTrigger()
{
	if (!bIsFiring)
	{
		return;
	}

	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
}

void APTGun::Fire()
{
	if (CurrentAmmo <= 0)
	{
		StopTrigger();
		return;
	}
	
	ConsumeBullet();
	ApplyRecoil();
	PlayMuzzleFlashEffectAndSound();
	
	FVector OutLocation;
	FRotator ViewRotation;
	GetOwnerController()->GetPlayerViewPoint(OutLocation, ViewRotation);
	
	GunFireComponent->FireProcess(ProjectileSpawnPoint->GetComponentLocation(), ViewRotation, GunData.Range, GunData.Damage);
	FireCount++;
}

void APTGun::ConsumeBullet()
{
	CurrentAmmo--;
	SetAmmo(CurrentAmmo, MaxAmmo);
}

void APTGun::ApplyRecoil()
{
	AController* OwnerController = GetOwnerController();
	APlayerController* PlayerController = Cast<APlayerController>(OwnerController);
	if (PlayerController == nullptr)
	{
		return;
	}

	const float YawRecoil =  GunData.Recoil / 5.f;
	const float PitchRecoil = GunData.Recoil;
		
	// 연사 횟수에 따른 증가량
	float FireCountRecoil = (FireCount / 3.f) * 0.2;
	FireCountRecoil = FMath::Min(FireCountRecoil, MAX_FIRECOUNT_RECOIL);
		
	// 이동 속도에 따른 증가량 
	auto CurrentVelocity = PlayerController->GetCharacter()->GetVelocity().Length();
	float VelocityRecoil = (CurrentVelocity < RECOIL_VELOCITY_THRESHOLD) ? 0.f : 0.5f;
		
	FRotator NewRotation = PlayerController->GetControlRotation();
	NewRotation.Yaw += (FMath::Sin(FMath::FRand() * 25.0f) + 0.5f) * (YawRecoil + FireCountRecoil + VelocityRecoil);
	NewRotation.Pitch += FMath::Sin(FMath::FRand() * 25.0f) * (PitchRecoil + FireCountRecoil + VelocityRecoil);

	PlayerController->SetControlRotation(NewRotation);
}

void APTGun::DamageToHitResult(FHitResult HitResult, int32 Damage, FVector ShotDirection)
{
	IPTFactionInterface* OwnerFaction = Cast<IPTFactionInterface>(GetOwner());
	IPTFactionInterface* TargetFaction = Cast<IPTFactionInterface>(HitResult.GetActor());
	if (OwnerFaction == nullptr || TargetFaction == nullptr)
	{
		return;
	}
	
	if (EFactionUtil::IsHostility(OwnerFaction->GetFaction(), TargetFaction->GetFaction()))
	{
		FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
		HitResult.GetActor()->TakeDamage(Damage, DamageEvent, GetOwnerController(), GetOwner());
	}
}

void APTGun::DamageToOverlapResults(const TArray<FOverlapResult>& OverlapResults, FVector Location, int32 InDamage)
{
	IPTFactionInterface* OwnerFaction = Cast<IPTFactionInterface>(GetOwner());

	for (FOverlapResult Target : OverlapResults)
	{
		IPTFactionInterface* TargetFaction = Cast<IPTFactionInterface>(Target.GetActor());
		if (OwnerFaction == nullptr || TargetFaction == nullptr)
		{
			return;
		}

		if (EFactionUtil::IsHostility(OwnerFaction->GetFaction(), TargetFaction->GetFaction()))
		{
			FRadialDamageEvent RadialDamageEvent;
			RadialDamageEvent.Params.BaseDamage = InDamage;
			RadialDamageEvent.Params.MinimumDamage = InDamage;
			RadialDamageEvent.Origin = Location;
			Target.GetActor()->TakeDamage(InDamage, RadialDamageEvent, GetOwnerController(), GetOwner());
		}
	}
}

float APTGun::Reloading(float AccelerationRate)
{
	FireCount = 0;
	
	AccelerationRate = FMath::Min(AccelerationRate, MAX_RELOAD_REDUCE_RATE);
	float result = GunData.ReloadTime * (1.0 - AccelerationRate);
	
	GetWorld()->GetTimerManager().ClearTimer(ReloadingTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(ReloadingTimerHandle, this, &APTGun::CompleteReload, result);

	OnStartReload.Broadcast(); 	
	
	return result;
}

void APTGun::CompleteReload()
{
	SetAmmo(MaxAmmo, MaxAmmo);
	
	OnCompleteReload.Broadcast();
}

void APTGun::SetAmmo(int32 InCurrentAmmo, int32 InMaxAmmo)
{
	CurrentAmmo = InCurrentAmmo;
	MaxAmmo = InMaxAmmo;

	OnChangeAmmo.Broadcast(CurrentAmmo, MaxAmmo);
}

void APTGun::PlayMuzzleFlashEffectAndSound()
{
	if (MuzzleFlashEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, Mesh, *MuzzleEffectSocketName, FVector(ForceInit), FRotator::ZeroRotator, FVector(0.75f));
	}
	
	if (MuzzleFlashSound)
	{
		UGameplayStatics::SpawnSoundAttached(MuzzleFlashSound, Mesh, *MuzzleEffectSocketName);
	}	
}

void APTGun::PlayImpactEffectAndSound(FHitResult Hit, FVector ShotDirection)
{
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
	}

	if (ImpactSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
	}
}
