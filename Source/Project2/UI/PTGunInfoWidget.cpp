#include "UI/PTGunInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPTGunInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentAmountTxt = Cast<UTextBlock>(GetWidgetFromName("CurrentAmount"));
	MaxAmountTxt = Cast<UTextBlock>(GetWidgetFromName("MaxAmount"));
	ReloadingImage = Cast<UImage>(GetWidgetFromName("ReloadingImg"));
	ReloadingImage->SetVisibility(ESlateVisibility::Hidden);
}

void UPTGunInfoWidget::UpdateBullet(int32 NewCurAmount, int32 NewMaxAmount)
{
	CurrentAmountTxt->SetText(FText::FromString(FString::FromInt(NewCurAmount)));
	MaxAmountTxt->SetText(FText::FromString(FString::FromInt(NewMaxAmount)));
}

void UPTGunInfoWidget::SetReloadingImg(uint8 InVisibility)
{
	if (InVisibility)
	{
		ReloadingImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ReloadingImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
