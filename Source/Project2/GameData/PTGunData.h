#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PTGunData.generated.h"

UENUM(BlueprintType)
enum class EGunType : uint8
{
	Rifle UMETA(DisplayName="Rifle"),		// 소총류
	Launcher UMETA(DisplayName="Launcher"),	// 유탄류
	Emitter UMETA(DisplayName="Emitter"),	// 방사기류
};

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	Single UMETA(DisplayName="Single"),		// 단발 
	Burst UMETA(DisplayName="Burst"),		// 점사
	Automatic UMETA(DisplayName="Auto"),	// 연발
};

USTRUCT(BlueprintType)
struct FPTGunData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPTGunData() : GunType(EGunType::Rifle), Damage(0), Range(0.0f), MaxAmmo(0), Recoil(0.0f), FireRate(0.0f), ReloadTime(0.0f), FireMode(EFireMode::Single), BurstCount(1), Penetration(1) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	EGunType GunType;	// 총 분류
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Range;	// 사거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 MaxAmmo;	// 탄창
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Recoil;	// 반동

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float FireRate;	// 발사 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float ReloadTime;	// 재장전 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	EFireMode FireMode;	// 발사 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 BurstCount; // 점사 모드에서 한번에 발사되는 탄 수 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 Penetration; // 관통수 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FString ProjectileName; // ProjectileData Key
	
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
	// FString ModelName;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
	// FString MuzzleflashEffectName;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
	// FString MuzzleflashSoundName;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
	// FString ImpactEffectName;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
	// FString ImpactSoundName;
	
};