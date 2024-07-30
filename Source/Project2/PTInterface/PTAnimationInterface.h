#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PTAnimationInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPTAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT2_API IPTAnimationInterface
{
	GENERATED_BODY()

public:
	virtual void OnNotifyAttack() = 0;
};
