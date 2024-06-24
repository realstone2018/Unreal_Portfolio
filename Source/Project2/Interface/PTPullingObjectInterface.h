#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PTPullingObjectInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPTPullingObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT2_API IPTPullingObjectInterface
{
	GENERATED_BODY()

public:
	virtual void Instantiate() = 0;
	virtual void Dispose() = 0;
};
