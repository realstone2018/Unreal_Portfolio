#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PTComponent/Equipment/PTEquipmentComponent.h"
#include "PTPlayerInputInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPTPlayerInputInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT2_API IPTPlayerInputInterface
{
	GENERATED_BODY()

public:
	virtual void StartAttack() = 0;
	virtual void StopAttack() = 0;
	virtual void DashAction() = 0;
	virtual void ReloadAction() = 0;

	virtual void EquipInput(EEquipType EquipType) = 0;
};
