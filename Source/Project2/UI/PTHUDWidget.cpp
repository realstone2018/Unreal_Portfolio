// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PTHUDWidget.h"
#include "PTHpBarWidget.h"
#include "PTCharacterStatWidget.h"
#include "PTEquipWeaponWidget.h"
#include "PTGunInfoWidget.h"
#include "Interface/PTCharactHUDInterface.h"

UPTHUDWidget::UPTHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UPTHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UPTHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UPTCharacterStatWidget>(GetWidgetFromName("WidgetCharacterStat"));
	ensure(CharacterStat);

	GunInfo = Cast<UPTGunInfoWidget>(GetWidgetFromName("WidgetGunInfo"));
	ensure(GunInfo);

	EquipWeapon = Cast<UPTEquipWeaponWidget>(GetWidgetFromName("WidgetEquipWeapon"));
	ensure(EquipWeapon);
	
	IPTCharactHUDInterface* HUDPawn = Cast<IPTCharactHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);	
	}
}

void UPTHUDWidget::UpdateStat(const FPTCharacterStat& BaseStat, const FPTCharacterStat& ModifierStat)
{
	FPTCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->SetMaxHp(TotalStat.MaxHp); 

	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UPTHUDWidget::UpdateHpBar(int NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UPTHUDWidget::UpdateGunAmmo(int NewCurAmmo, int NewMaxAmmo)
{
	GunInfo->UpdateBullet(NewCurAmmo, NewMaxAmmo);
}

void UPTHUDWidget::UpdateGunReloadImg(bool InVisibility)
{
	GunInfo->SetReloadingImg(InVisibility);
}

void UPTHUDWidget::UpdateEquipWeapon(bool IsEquipMain)
{
	IsEquipMain ? EquipWeapon->EquipMainWeapon() : EquipWeapon->EquipSubWeapon();
}

