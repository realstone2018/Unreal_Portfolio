#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameData/PTGunData.h"
#include "Gun.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStartReloadDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCompleteReloadDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeAmmo, int /*CurrentAmmo*/, int /*MaxAmmo*/)

UCLASS()
class PROJECT2_API AGun : public AActor
{
	GENERATED_BODY()

// Stat
public:	
	AGun();
	
	FORCEINLINE void SetGunData(const FPTGunData& InGunData);

	FOnChangeAmmo OnChangeAmmo;
	
private:
	UPROPERTY(EditAnywhere, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	FPTGunData GunData;
	
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	int32 CurrentAmmo;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	int32 MaxAmmo;
	
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

	FORCEINLINE int GetCurrentAmmo() { return CurrentAmmo; }
	FORCEINLINE int GetMaxAmmo() { return MaxAmmo; }
	
private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	void Fire();
	void ConsumeBullet();
	void ApplyRecoil();
	
// Reload
public:
	float Reloading(float AccelerationRate);
	void CancelReload(); //TODO: 구르기, 회피 등의 캐릭터의 다른 액션으로 캔슬

	FOnStartReloadDelegate OnStartReload;
	FOnCompleteReloadDelegate OnCompleteReload;

private:
	void CompleteReload(); //TODO: 보급 획득시 즉시 장전 -> 즉시 Reloading 함수 or Reloading 인자로 1.0 넘기기
	void SetAmmo(int InCurrentAmmo, int InMaxAmmo);

	
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
