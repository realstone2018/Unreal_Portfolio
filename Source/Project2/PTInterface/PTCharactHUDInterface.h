#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PTCharactHUDInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPTCharactHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT2_API IPTCharactHUDInterface
{
	GENERATED_BODY()

public:
	virtual void SetupHUDWidget(class UPTHUDWidget* InHUDWidget) = 0;
	virtual void SetupHpBarWidget(class UPTUserWidget* InUserWidget) = 0;
};
