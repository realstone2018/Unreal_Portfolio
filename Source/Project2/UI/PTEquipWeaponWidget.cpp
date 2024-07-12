#include "PTEquipWeaponWidget.h"

const FLinearColor UPTEquipWeaponWidget::ShowSelectColor = FLinearColor(1.f, 1.f, 1.f, 0.8f);
const FLinearColor UPTEquipWeaponWidget::HideSelectColor = FLinearColor(1.f, 1.f, 1.f, 0.f);
const FLinearColor UPTEquipWeaponWidget::SelectWeaponColor = FLinearColor(1.f, 1.f, 1.f, 0.8f);
const FLinearColor UPTEquipWeaponWidget::DeselectWeaponColor = FLinearColor(1.f, 1.f, 1.f, 0.4f);

void UPTEquipWeaponWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainWeapon = Cast<UImage>(GetWidgetFromName("MainWeaponImg"));
	MainSelect = Cast<UImage>(GetWidgetFromName("MainSelectImg"));

	SubWeapon = Cast<UImage>(GetWidgetFromName("SubWeaponImg"));
	SubSelect = Cast<UImage>(GetWidgetFromName("SubSelectImg"));
}

void UPTEquipWeaponWidget::EquipMainWeapon()
{
	MainWeapon->SetColorAndOpacity(SelectWeaponColor);
	MainSelect->SetColorAndOpacity(ShowSelectColor);

	SubWeapon->SetColorAndOpacity(DeselectWeaponColor);
	SubSelect->SetColorAndOpacity(HideSelectColor);
}

void UPTEquipWeaponWidget::EquipSubWeapon()
{
	MainWeapon->SetColorAndOpacity(DeselectWeaponColor);
	MainSelect->SetColorAndOpacity(HideSelectColor);

	SubWeapon->SetColorAndOpacity(SelectWeaponColor);
	SubSelect->SetColorAndOpacity(ShowSelectColor);
}
