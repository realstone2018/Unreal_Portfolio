#pragma once

#include "CoreMinimal.h"
#include "Character/PTCharacterBase.h"
#include "PTComponent/Character/PTMonsterStatComponent.h"
#include "PTInterface/PTPullingObjectInterface.h"
#include "PTInterface/PTMonsterAIInterface.h"
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
	
	virtual void PostInitializeComponents() override;
	
	virtual void Initialize() override;
	virtual void Terminate() override;

// Stat
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPTMonsterStatComponent> MonsterStat;

	virtual UPTCharacterStatComponent* GetStatComponent() override;

// Widget
	virtual bool GetShouldDisplayHpBar() override { return true; }


#pragma region Battle
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation") 
	TObjectPtr<class UAnimMontage> AttackMontage;

	void PlayAttackMontage();

	virtual void OnNotifyAttack() override;
	
	void Attack();
	FAICharacterAttackFinished OnAttackFinished;

	virtual void Dash() override;
	
private:
	//몽타주가 종료될 때 호출, 몽타주에 설정된 delegate로 바로 호출되도록 파라미터 세팅 (UAnimMontage.OnMontageEnd 딜리게이트의 파라미터와 동일하게)
	void EndAttackMontage(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	
public:
	FOnDeadDelegate OnDead;
	
protected:
	virtual void Dead() override;


#pragma endregion


#pragma region AI
	FORCEINLINE float GetAIDetectPlayerRange() override { return 800.f;  }
	FORCEINLINE float GetAIDetectWallRange() override {return 2000; }
	FORCEINLINE virtual float GetAIAttackRange() override { return MonsterStat->GetAttackRange(); }
	FORCEINLINE virtual float GetAITurnSpeed() override { return 10.f; }
	FORCEINLINE virtual float GetAIAttackCooldown() override { return MonsterStat->GetAttackCooldown(); }
	
	FORCEINLINE virtual void JumpByAI() override { Dash(); }
	
	FORCEINLINE virtual void AttackByAI(float& AttackCooldown) override { PlayAttackMontage(); AttackCooldown = MonsterStat->GetAttackCooldown(); }
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
#pragma endregion 
};
