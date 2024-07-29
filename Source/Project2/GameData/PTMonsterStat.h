#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PTMonsterStat.generated.h"

USTRUCT(BlueprintType)
struct FPTMonsterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPTMonsterStat() : MaxHp(0), Attack(0), AttackRange(0.f), AttackRadius(0.f), AttackCooldown(0.f), MovementSpeed(0.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;
	
	FPTMonsterStat operator+(const FPTMonsterStat& Other) const
	{
		FPTMonsterStat Result;

		Result.MaxHp = this->MaxHp + Other.MaxHp;
		Result.Attack = this->Attack + Other.Attack;
		Result.AttackRange = this->AttackRange + Other.AttackRange;
		Result.AttackRadius = this->AttackRadius + Other.AttackRadius;
		Result.AttackCooldown = this->AttackCooldown + Other.AttackCooldown;
		Result.MovementSpeed = this->MovementSpeed + Other.MovementSpeed;
		
		return Result;
	}
};