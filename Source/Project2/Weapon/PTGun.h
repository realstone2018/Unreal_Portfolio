#pragma once

#include "CoreMinimal.h"
#include "GameData/PTGunData.h"
#include "PTGun.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStartReloadDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCompleteReloadDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeAmmo, int32 /*CurrentAmmo*/, int32 /*MaxAmmo*/)

UCLASS()
class PROJECT2_API APTGun : public AActor
{
	GENERATED_BODY()

public:	
	APTGun();

	AController* GetOwnerController() const;


#pragma region Stat
	FORCEINLINE void Init(const FString GunDataKey);

private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPTGunData GunData;
	
	UPROPERTY(VisibleAnywhere, Category = Stat)
	int32 CurrentAmmo;	// 현재 탄창

	UPROPERTY(VisibleAnywhere, Category = Stat)
	int32 MaxAmmo;	// 최대 탄창
	
	UPROPERTY(EditAnywhere, Category = Stat)
	float RecoilAmount; // 반동의 크기

	UPROPERTY(EditAnywhere, Category = Stat)
	float RecoilResetSpeed; // 반동이 원래 위치로 돌아가는 속도
	
#pragma endregion 


#pragma region Fire
public:
	void PullTrigger();
	void StopTrigger();

	void DamageToHitResult(FHitResult HitResult, int32 Damage, FVector ShotDirection);
	void DamageToOverlapResults(const TArray<FOverlapResult>& OverlapResults, FVector Location, int32 InDamage);
	
	FORCEINLINE uint8 GetIsFiring() { return bIsFiring; }
	FORCEINLINE int32 GetCurrentAmmo() { return CurrentAmmo; }
	FORCEINLINE int32 GetMaxAmmo() { return MaxAmmo; }
	
private:
	void Fire();
	void ConsumeBullet();
	void ApplyRecoil();
	
	UPROPERTY(VisibleAnywhere, Category = Components)
	TObjectPtr<class UGunFireComponent> GunFireComponent; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> ProjectileSpawnPoint;
	
	uint8 bIsFiring : 1;
	int32 FireCount;

	FTimerHandle FireRateTimerHandle;

	const float MAX_FIRECOUNT_RECOIL = 2.0f;
	const float RECOIL_VELOCITY_THRESHOLD = 300.f;

#pragma endregion

	
#pragma region Reload
public:
	FOnStartReloadDelegate OnStartReload;
	FOnCompleteReloadDelegate OnCompleteReload;
	FOnChangeAmmo OnChangeAmmo;

	float Reloading(float AccelerationRate);

private:
	void CompleteReload();
	void SetAmmo(int32 InCurrentAmmo, int32 InMaxAmmo);

	FTimerHandle ReloadingTimerHandle;
	
	const int MAX_RELOAD_REDUCE_RATE = 0.5F;

#pragma endregion
	

#pragma region Effect, Sound
public:
	void PlayMuzzleFlashEffectAndSound();
	void PlayImpactEffectAndSound(FHitResult Hit, FVector ShotDirection);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> MuzzleFlashEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> MuzzleFlashSound;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	FString MuzzleEffectSocketName = "MuzzleFlashSocket";

#pragma endregion
	
};
