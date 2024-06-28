#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PTMonsterStat.generated.h"

USTRUCT(BlueprintType)
struct FPTMonsterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPTMonsterStat() : MaxHp(0), Attack(0), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

	//구조체 멤버변수 전부 float라는 가정하에 sizeof를 float으로 나눠 멤버변수의 수 get, 이를 통해 for문
	//멤버변수가 몇개가 추가되든, float기만 하면 항상 동작
	FPTMonsterStat operator+(const FPTMonsterStat& Other) const
	{
		FPTMonsterStat Result;

		Result.MaxHp = this->MaxHp + Other.MaxHp;
		Result.Attack = this->Attack + Other.Attack;
		Result.AttackRange = this->AttackRange + Other.AttackRange;
		Result.AttackSpeed = this->AttackSpeed + Other.AttackSpeed;
		Result.MovementSpeed = this->MovementSpeed + Other.MovementSpeed;
		
		return Result;
	}
};