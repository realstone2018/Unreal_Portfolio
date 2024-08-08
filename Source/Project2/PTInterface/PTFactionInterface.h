#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PTFactionInterface.generated.h"

UENUM(BlueprintType)
enum class EFaction : uint8
{
	None UMETA(DisplayName="None"),
	Player UMETA(DisplayName="Player"),
	Monster UMETA(DisplayName=Monster),
};

class EFactionUtil
{
public:
	FORCEINLINE static uint8 IsNoneFaction(EFaction Faction)
	{
		return Faction == EFaction::None;
	}
	
	FORCEINLINE static uint8 IsHostility(EFaction A, EFaction B)
	{
		if (A == EFaction::None || B == EFaction::None)
		{
			return false;
		}

		return A != B;
	}
};

UINTERFACE(MinimalAPI)
class UPTFactionInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT2_API IPTFactionInterface
{
	GENERATED_BODY()

public:
	virtual EFaction GetFaction() = 0;
	
};
