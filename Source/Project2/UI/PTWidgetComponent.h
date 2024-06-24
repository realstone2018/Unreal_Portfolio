#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PTWidgetComponent.generated.h"

UCLASS()
class PROJECT2_API UPTWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void InitWidget() override;
};
