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
	UFUNCTION(BlueprintCallable)
	void UpdateBullet(int32 NewCurAmount, int32 NewMaxAmount);
	void SetReloadingImg(uint8 InVisibility);
	
private:
	UPROPERTY()
	TObjectPtr<class UTextBlock> CurrentAmountTxt;

	UPROPERTY()
	TObjectPtr<class UTextBlock> MaxAmountTxt;

	UPROPERTY()
	TObjectPtr<class UImage> ReloadingImage;
};
