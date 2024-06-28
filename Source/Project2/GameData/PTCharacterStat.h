#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PTCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FPTCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPTCharacterStat() : MaxHp(0), Attack(0), Armor(0), DamageReduceRatio(0.0f), MovementSpeed(0.0f), RangeIncreaseRatio(0.0f), ReloadReduceRatio(0.0f), Penetration(0) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 MaxHp;	// 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 Attack;	// 추가 공격력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 Armor;	// 방어력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float DamageReduceRatio;	// 받는 데미지 감소률

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;	// 이동속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float RangeIncreaseRatio;	// 사거리 증가률
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float ReloadReduceRatio;	// 재장전속도 감소률
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 Penetration;	// 관통

	
	FPTCharacterStat operator+(const FPTCharacterStat& Other) const
	{
		FPTCharacterStat Result;

		Result.MaxHp = this->MaxHp + Other.MaxHp;
		Result.Attack = this->Attack + Other.Attack;
		Result.Armor = this->Armor + Other.Armor;
		Result.DamageReduceRatio = this->DamageReduceRatio + Other.DamageReduceRatio;
		Result.MovementSpeed = this->MovementSpeed + Other.MovementSpeed;
		Result.RangeIncreaseRatio = this->RangeIncreaseRatio + Other.RangeIncreaseRatio;
		Result.ReloadReduceRatio = this->ReloadReduceRatio + Other.ReloadReduceRatio;
		Result.Penetration = this->Penetration + Other.Penetration;

		return Result;
	}
};