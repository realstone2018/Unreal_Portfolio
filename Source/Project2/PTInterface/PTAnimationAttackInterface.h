#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PTAnimationAttackInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPTAnimationAttackInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT2_API IPTAnimationAttackInterface
{
	GENERATED_BODY()

public:
	virtual void OnNotifyAttack() = 0;
};
