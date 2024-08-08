#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PTMonsterAIInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPTAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAICharacterAttackFinished);

class PROJECT2_API IPTAIInterface
{
	GENERATED_BODY()

public:
	virtual float GetAIDetectPlayerRange() = 0;
	virtual AActor* GetLastAttacker() = 0;
	
	virtual float GetAIDetectWallRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;
	virtual float GetAIAttackCooldown() = 0;
	
	virtual void JumpByAI() = 0;

	virtual float GetHpRatio() = 0;
	
	virtual void AttackByAI(float& AttackCooldown) = 0;
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;
};
