#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PTHUDInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPTHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT2_API IPTHUDInterface
{
	GENERATED_BODY()

public:
	virtual void SetupHUDWidget(class UPTHUDWidget* InHUDWidget) = 0;
	virtual void SetupHpBarWidget(class UPTUserWidget* InUserWidget) = 0;
};
