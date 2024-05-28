#pragma once

#include "CoreMinimal.h"
#include "Character/PTMonster.h"
#include "PTScorch.generated.h"

UCLASS()
class PROJECT2_API APTScorch : public APTMonster
{
	GENERATED_BODY()

public:
	APTScorch();

	virtual void BeginPlay() override;
};
