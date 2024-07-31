#include "PTGun.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PTComponent/Equipment/RifleFireComponent.h"
#include "PTComponent/Equipment/LauncherFireComponent.h"

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

void APTGun::SetGunData(const FPTGunData& InGunData)
{
	GunData = InGunData;
	MaxAmmo = GunData.MaxAmmo;
	CurrentAmmo = MaxAmmo;

	switch (GunData.GunType)
	{
		case EGunType::Rifle:
			GunFireComponent = NewObject<URifleFireComponent>(this, URifleFireComponent::StaticClass(), TEXT("RifleFireComponent"));
			break;

		case EGunType::Launcher:
			GunFireComponent = NewObject<ULauncherFireComponent>(this, ULauncherFireComponent::StaticClass(), TEXT("LauncherFireComponent"));
			break;

		case EGunType::Emitter:
			break;
	}
	
	GunFireComponent->RegisterComponent();	// 엔진에 생성한 컴포넌트를 인식
	GunFireComponent->Init(this);
	GunFireComponent->OnHitTracing.BindUObject(this, &APTGun::PlayImpactEffectAndSound);
}

uint8 APTGun::PullTrigger()
{
	if (CurrentAmmo <= 0)
	{
		return false;
	}
	
	if (!bIsFiring)
	{
		FireCount = 0;
		bIsFiring = true;
		
		Fire();
		
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APTGun::Fire, GunData.FireRate, true);
	}

	return true;
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
	
	GunFireComponent->FireProcess(ProjectileSpawnPoint->GetComponentLocation(), GunData.Range, GunData.Damage, GunData.ProjectileName);
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
