#pragma once
#include "Character/PTMonster.h"
#include "PTActor/PTProjectile.h"

UENUM(BlueprintType)
enum class EPoolType : uint8
{
	None = 0,
	Monster UMETA(DisplayName = "Monster"),
	Projectile UMETA(DisplayName = "Projectile"),
};

class EPoolTypeUtil
{
public:
	FORCEINLINE static EPoolType ClassToPoolType(UClass* StaticClass)
	{
		if (StaticClass == APTMonster::StaticClass())
		{
			return EPoolType::Monster;
		}
		
		if (StaticClass == APTProjectile::StaticClass())
		{
			return EPoolType::Projectile;
		}
		
		return EPoolType::None;
	}
};
