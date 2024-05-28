#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameData/PTGunData.h"
#include "Gun.generated.h"

DECLARE_DELEGATE(FOnCompleteReloadDelegate);
//TODO: 누르고있으면 자동 재장전할거면 델리게이트로

UCLASS()
class PROJECT2_API AGun : public AActor
{
	GENERATED_BODY()

// Stat
public:	
	AGun();
	
	FORCEINLINE void SetGunData(const FPTGunData& InGunData);

private:
	UPROPERTY(EditAnywhere, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	FPTGunData GunData;
	
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	int32 bullet;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	float RecoilAmount; // 반동의 크기

	UPROPERTY(EditAnywhere, Category = "Stat")
	float RecoilResetSpeed; // 반동이 원래 위치로 돌아가는 속도

	bool bIsFiring;
	int FireCount;

// Fire
public:
	bool PullTrigger();
	void StopTrigger();

private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	void Fire();
	void ConsumeBullet();
	void ApplyRecoil();
	
	FORCEINLINE void SetBullet(int InBullet)
	{
		UE_LOG(LogTemp, Display, TEXT("Gun::SetBullet() - %d"), InBullet);
		bullet = InBullet;
	}
	
	
// Reload
public:
	float Reloading(float AccelerationRate);
	void CancelReload(); //TODO: 구르기, 회피 등의 캐릭터의 다른 액션으로 캔슬

	FOnCompleteReloadDelegate OnCompleteReload;

private:
	void CompleteReload(); //TODO: 보급 획득시 즉시 장전 -> 즉시 Reloading 함수 or Reloading 인자로 1.0 넘기기

	
private:
	AController* GetOwnerController() const;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleFlashSound;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;
	
	FTimerHandle FireRateTimerHandle;
	FTimerHandle ReloadingTimerHandle;
	
};
