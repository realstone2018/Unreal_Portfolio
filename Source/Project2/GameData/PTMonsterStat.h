#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PTMonsterStat.generated.h"

USTRUCT(BlueprintType)
struct FPTMonsterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPTMonsterStat() : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

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
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FPTMonsterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FPTMonsterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};