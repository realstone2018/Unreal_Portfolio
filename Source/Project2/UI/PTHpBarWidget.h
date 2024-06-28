#pragma once

#include "CoreMinimal.h"
#include "UI/PTUserWidget.h"
#include "PTHpBarWidget.generated.h"

UCLASS()
class PROJECT2_API UPTHpBarWidget : public UPTUserWidget
{
	GENERATED_BODY()

public:
	UPTHpBarWidget();
		 
protected:
	//해당 함수가 호출되는 타이밍은 UI관련된 모든 기능들이 거의 초기화 완료 
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }

	//TODO: 
	//14-5 UpdateStat()으로 대체
	// void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
	// FString GetHpStatText();
	
	void UpdateHpBar(int NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	//TODO:
	// //14-5
	// UPROPERTY()
	// TObjectPtr<class UTextBlock> HpStat;
	//
	// //14-5
	// UPROPERTY()
	// float CurrentHp;
	
	UPROPERTY()
	float MaxHp;
};
