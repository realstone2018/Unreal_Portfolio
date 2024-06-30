#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PTGunInfoWidget.generated.h"

UCLASS()
class PROJECT2_API UPTGunInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateBullet(int NewCurAmount, int NewMaxAmount);
	void SetReloadingImg(bool InVisibility);
	
private:
	UPROPERTY()
	TObjectPtr<class UTextBlock> CurrentAmountTxt;

	UPROPERTY()
	TObjectPtr<class UTextBlock> MaxAmountTxt;

	UPROPERTY()
	TObjectPtr<class UImage> ReloadingImage;
};
