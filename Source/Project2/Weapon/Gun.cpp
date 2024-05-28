#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RandomStream.h"

AGun::AGun()
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
	
	//TODO: OwnerController를 캐싱하는게 나을수도 
}

void AGun::SetGunData(const FPTGunData& InGunData)
{
	GunData = InGunData;
	bullet = GunData.MaxAmmo;
}

bool AGun::PullTrigger()
{
	UE_LOG(LogTemp, Display, TEXT("Gun::PullTrigger() - %f"), UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	UE_LOG(LogTemp, Display, TEXT("Gun::PullTrigger() - %s"), bIsFiring ? TEXT("True") : TEXT("False"));

	//TODO: 재장전중이거나 발사도중 재 입력이 들어오는등 PullTrigger가 연속으로 두번호출된다던가 이런류의 에러처리 필요, 꼬이지 않게 간결하고 정확한 코드 필요
	if (bullet <= 0)
	{
		return false;
	}
	
	if (!bIsFiring)
	{
		FireCount = 0;
		bIsFiring = true;
		Fire(); // 즉시 발사하고, 타이머 시작
		
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AGun::Fire, GunData.FireRate, true);
	}

	return true;
}

void AGun::StopTrigger()
{
	UE_LOG(LogTemp, Display, TEXT("Gun::StopTrriger() - %f"), UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	UE_LOG(LogTemp, Display, TEXT("Gun::StopTrriger() - %s"), bIsFiring ? TEXT("True") : TEXT("False"));
	
	if (bIsFiring)
	{
		bIsFiring = false;
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);

		UE_LOG(LogTemp, Display, TEXT("Gun::StopTrriger() - ClearTimer"));
	}
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	//컨트롤러를 가져와, 컨트롤러를 통해 뷰포트의 Location과 Rotation을 가져온다.
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}
	
	// 폰이 보고있는 시야의 시작 위치와 회전방향을 가져온다. (카메라가 붙어있는 경우 카메라 베이스로, 없는 경우는 모르겠다.)
	FVector OutLocation;
	FRotator OutRotation;
	OwnerController->GetPlayerViewPoint(OutLocation, OutRotation);
	
	FVector End = OutLocation + OutRotation.Vector() * GunData.Range;
	ShotDirection =	-OutRotation.Vector();

	if (MuzzleFlashEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, Mesh, TEXT("MuzzleFlashSocket"));
	}
	
	if (MuzzleFlashSound)
	{
		UGameplayStatics::SpawnSoundAttached(MuzzleFlashSound, Mesh, TEXT("MuzzleFlashSocket"));
	}


	DrawDebugLine(GetWorld(), OutLocation, End, FColor::Red, true, 5.f);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	
	//어딘가에 부딪쳐야만 True반환
	return GetWorld()->LineTraceSingleByChannel(Hit, OutLocation, End, ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}
	
	return OwnerPawn->GetController();
}


//TODO: 함수 분리 필요해보임
void AGun::Fire()
{
	if (bullet <= 0)
	{
		StopTrigger();
		return;
	}
	
	ConsumeBullet();
	ApplyRecoil();
	FireCount++;
	
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (!bSuccess)
	{
		return;
	}
	
	AActor* HitActor = Hit.GetActor();

	UE_LOG(LogTemp, Display, TEXT("Gun::Fire() - %s"), *HitActor->GetName());

	if (HitActor != nullptr)
	{
		AController* OwnerController = GetOwnerController();
		if (OwnerController == nullptr)
		{
			return;
		}
				
		FPointDamageEvent DamageEvent(GunData.Damage, Hit, ShotDirection, nullptr);
		HitActor->TakeDamage(GunData.Damage, DamageEvent, OwnerController, this);
	}

	//TODO: Emitter 컨트롤러 만들어서 null체크 랩핑해주기 + SpawnEmitterAtLocation내부코드에 널체크 없는지 그냥 널 스폰해보기  
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
	}

	if (ImpactSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
	}
	
	DrawDebugPoint(GetWorld(), Hit.Location, 10, FColor::Red, true);
}

void AGun::ConsumeBullet()
{
	bullet--;

	UE_LOG(LogTemp, Display, TEXT("Gun::ConsumeBullet() bullet: %d"), bullet);

	//TODO:  탄피 애니메이션 && 가능하다면 총기 애니메이션도

		
}

void AGun::ApplyRecoil()
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

		UE_LOG(LogTemp, Display, TEXT("Gun::ApplyRecoil() - YawRecoil: %f  FireCountAmp: %f   VelocityAmp: %f"), YawRecoil, FireCountAmp, VelocityAmp);

		
		FRotator NewRotation = PlayerController->GetControlRotation();
		NewRotation.Yaw += (FMath::Sin(FMath::FRand() * 25.0f) + 0.5f) * (YawRecoil + FireCountAmp + VelocityAmp);
		NewRotation.Pitch += FMath::Sin(FMath::FRand() * 25.0f) * (PitchRecoil + FireCountAmp + VelocityAmp);

		PlayerController->SetControlRotation(NewRotation);
	}
}

#pragma region Reload
float AGun::Reloading(float AccelerationRate)
{
	FireCount = 0;
	
	UE_LOG(LogTemp, Display, TEXT("Gun::Reloading()"));

	float MaxAccelerationRate = 0.5f; // 최대 50% 감소, GameConst값으로 빼기 or 캐릭터 스탯에서 제어
	AccelerationRate = FMath::Min(AccelerationRate, MaxAccelerationRate);
	
	float result = GunData.ReloadTime * (1.0 - AccelerationRate);

	//TODO: 총기 재장전 관련 애니메이션 있다면 재생, 타이머 작동

	GetWorld()->GetTimerManager().ClearTimer(ReloadingTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(ReloadingTimerHandle, this, &AGun::CompleteReload, result);

	return result;
}

void AGun::CancelReload()
{
}

void AGun::CompleteReload()
{
	SetBullet(GunData.MaxAmmo);
	
	if (OnCompleteReload.IsBound())
	{
		OnCompleteReload.Execute();
	}
}
#pragma endregion 

