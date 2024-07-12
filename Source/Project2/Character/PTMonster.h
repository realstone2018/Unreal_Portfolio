#pragma once

#include "CoreMinimal.h"
#include "Character/PTCharacterBase.h"
#include "Interface/PTPullingObjectInterface.h"
#include "PTMonster.generated.h"

UENUM()
enum class MonsterType : uint8
{
	None = 0,
	Scorch,
};

DECLARE_DELEGATE_OneParam(FOnDeadDelegate, AActor* /*this*/);

UCLASS()
class PROJECT2_API APTMonster : public APTCharacterBase, public IPTPullingObjectInterface
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

	FOnDeadDelegate OnDead;
	
protected:
	virtual void Dead() override;

	
private:
	//몽타주가 종료될 때 호출, 몽타주에 설정된 delegate로 바로 호출되도록 파라미터 세팅 (UAnimMontage.OnMontageEnd 딜리게이트의 파라미터와 동일하게)
	void EndAttackMontage(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

#pragma endregion
	
};
