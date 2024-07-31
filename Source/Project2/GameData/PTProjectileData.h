#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PTProjectileData.generated.h"

USTRUCT(BlueprintType)
struct FPTProjectileData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPTProjectileData() : Radius(0.f), ExplosionRadius(0.f), MoveSpeed(0.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Radius;	// 충돌 반경
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float ExplosionRadius;	// 폭발 반경
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MoveSpeed;	// 속도


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