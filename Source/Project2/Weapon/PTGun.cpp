#include "PTGun.h"
#include "AsyncTreeDifferences.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RandomStream.h"
#include "PTComponent/Equipment//RifleFireComponent.h"
#include "PTComponent/Equipment//LauncherFireComponent.h"

APTGun::APTGun()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	bIsFiring = false;
	
	RecoilAmount = 1.0f; // 기본 반동 크기
	RecoilResetSpeed = 5.0f; // 기본 반동 복구 속도
	FireCount = 0;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
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

	// 엔진에 생성한 컴포넌트를 인식
	GunFireComponent->RegisterComponent();
	GunFireComponent->Init(this, ProjectileClass);
	GunFireComponent->OnHitTracing.BindUObject(this, &APTGun::PlayImpactEffectAndSound);
}

uint8 APTGun::PullTrigger()
{
	//UE_LOG(LogTemp, Display, TEXT("Gun::PullTrigger() - %f"), UGameplayStatics::GetRealTimeSeconds(GetWorld()));

	//TODO: 재장전중이거나 발사도중 재 입력이 들어오는등 PullTrigger가 연속으로 두번호출된다던가 이런류의 에러처리 필요, 꼬이지 않게 간결하고 정확한 코드 필요
	//TODO: 총알이 없을때 재장전 대신 딸각딸각 처리가 나을수도 
	if (CurrentAmmo <= 0)
	{
		return false;
	}
	
	if (!bIsFiring)
	{
		FireCount = 0;
		bIsFiring = true;
		Fire(); // 즉시 발사하고, 타이머 시작
		
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APTGun::Fire, GunData.FireRate, true);
	}

	return true;
}

void APTGun::StopTrigger()
{
	if (bIsFiring)
	{
		bIsFiring = false;
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
	}
}

//TODO: 멀티플레이 대비, 사용하는 플레이어가 변경되어도 정상동작할지 체크 필요
AController* APTGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}
	
	return OwnerPawn->GetController();
}


//TODO: 함수 분리 필요해보임
void APTGun::Fire()
{
	if (CurrentAmmo <= 0)
	{
		StopTrigger();
		return;
	}
	
	ConsumeBullet();
	ApplyRecoil();
	FireCount++;

	if (MuzzleFlashEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, Mesh, TEXT("MuzzleFlashSocket"));
	}
	
	if (MuzzleFlashSound)
	{
		UGameplayStatics::SpawnSoundAttached(MuzzleFlashSound, Mesh, TEXT("MuzzleFlashSocket"));
	}
	
	 GunFireComponent->FireProcess(ProjectileSpawnPoint->GetComponentLocation(), GunData.Range, GunData.Damage);
}

void APTGun::ConsumeBullet()
{
	CurrentAmmo--;
	SetAmmo(CurrentAmmo, MaxAmmo);
	
	//TODO:  탄피 애니메이션 && 가능하다면 총기 애니메이션도


}

void APTGun::ApplyRecoil()
{
	AController* OwnerController = GetOwnerController();
	
	APlayerController* PlayerController = Cast<APlayerController>(OwnerController);
	if (PlayerController)
	{
		float YawRecoil =  GunData.Recoil / 5.f;
		float PitchRecoil = GunData.Recoil;
		
		// 연사 횟수에 따른 증가량
		float FireCountAmp = (FireCount / 3.f) * 0.2;
		FireCountAmp = FMath::Min(FireCountAmp, 2.0f);  //TODO: GameConstnat에 MaxFireCountAmp 상수로 빼기 
		
		// 이동 속도에 따른 증가량 
		auto CurrentVelocity = PlayerController->GetCharacter()->GetVelocity().Length();
		float VelocityAmp = (CurrentVelocity < 300) ? 0.f : 0.5f;
		
		FRotator NewRotation = PlayerController->GetControlRotation();
		NewRotation.Yaw += (FMath::Sin(FMath::FRand() * 25.0f) + 0.5f) * (YawRecoil + FireCountAmp + VelocityAmp);
		NewRotation.Pitch += FMath::Sin(FMath::FRand() * 25.0f) * (PitchRecoil + FireCountAmp + VelocityAmp);

		PlayerController->SetControlRotation(NewRotation);
	}
}

void APTGun::PlayImpactEffectAndSound(FHitResult Hit, FVector ShotDirection)
{
	//TODO: Emitter 컨트롤러 만들어서 null체크 랩핑해주기 + SpawnEmitterAtLocation내부코드에 널체크 없는지 그냥 널 스폰해보기  
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
	}

	if (ImpactSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
	}
}


#pragma region Reload
float APTGun::Reloading(float AccelerationRate)
{
	FireCount = 0;
	
	float MaxAccelerationRate = 0.5f; // 최대 50% 감소, GameConst값으로 빼기 or 캐릭터 스탯에서 제어
	AccelerationRate = FMath::Min(AccelerationRate, MaxAccelerationRate);
	
	float result = GunData.ReloadTime * (1.0 - AccelerationRate);

	//TODO: 총기 재장전 관련 애니메이션 있다면 재생, 타이머 작동

	GetWorld()->GetTimerManager().ClearTimer(ReloadingTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(ReloadingTimerHandle, this, &APTGun::CompleteReload, result);

	OnStartReload.Broadcast();
	
	return result;
}

void APTGun::CancelReload()
{
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
#pragma endregion 

