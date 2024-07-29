#pragma once

#include "CoreMinimal.h"
#include "UI/PTUserWidget.h"
#include "PTHpBarWidget.generated.h"

UCLASS()
class PROJECT2_API UPTHpBarWidget : public UPTUserWidget
{
	GENERATED_BODY()

public:
	UPTHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(int32 NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
};
