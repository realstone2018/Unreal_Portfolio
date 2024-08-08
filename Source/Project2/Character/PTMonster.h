#pragma once

#include "CoreMinimal.h"
#include "Character/PTCharacterBase.h"
#include "PTInterface/PTPullingObjectInterface.h"
#include "PTInterface/PTMonsterAIInterface.h"
#include "PTComponent/Character/PTMonsterStatComponent.h"
#include "PTMonster.generated.h"

UENUM()
enum class MonsterType : uint8
{
	None = 0,
	Scorch,
};

DECLARE_DELEGATE_OneParam(FOnDeadDelegate, AActor* /*this*/);

UCLASS()
class PROJECT2_API APTMonster : public APTCharacterBase, public IPTPullingObjectInterface, public IPTAIInterface
{
	GENERATED_BODY()

public:
	APTMonster();
	
	virtual void Initialize(FName DataKey) override;
	virtual void Terminate() override;


#pragma region Component
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPTMonsterStatComponent> MonsterStat;

	virtual UPTCharacterStatComponent* GetStatComponent() override;

#pragma endregion


#pragma region Battle
public:
	FOnDeadDelegate OnDead;
	FAICharacterAttackFinished OnAttackFinished;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void Dead() override;
	virtual void Dash();
	void Attack();
	
	void PlayAttackMontage();
	virtual void OnNotifyAttack() override;
	void EndAttackMontage(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation") 
	TObjectPtr<class UAnimMontage> AttackMontage;
	
private:
	UPROPERTY()
	TArray<AActor*> HitTargets;

#pragma endregion


#pragma region AI
private:
	FORCEINLINE float GetAIDetectPlayerRange() override { return 2000.f;  }
	AActor* GetLastAttacker() override { return LastAttacker;; }

	UPROPERTY()
	TObjectPtr<AActor> LastAttacker;
	
	FORCEINLINE float GetAIDetectWallRange() override {return 1500; }
	virtual float GetAIAttackRange() override { return MonsterStat->GetAttackRange(); }
	virtual float GetAITurnSpeed() override { return 10.f; }
	virtual float GetAIAttackCooldown() override { return MonsterStat->GetAttackCooldown(); }

	virtual float GetHpRatio() override;
	
	virtual void JumpByAI() override { Dash(); }
	
	virtual void AttackByAI(float& AttackCooldown) override;
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;

#pragma endregion

#pragma region Widget
public:
	virtual uint8 GetShouldDisplayHpBar() override { return true; }

#pragma endregion
	
};
