#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/PTCharacterStat.h"
#include "PTHUDWidget.generated.h"

UCLASS()
class PROJECT2_API UPTHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FPTCharacterStat& BaseStat, const FPTCharacterStat& ModifierStat);
	void UpdateHpBar(int NewCurrentHp);
	
	void UpdateGunAmmo(int NewCurAmmo, int NewMaxAmmo);
	void UpdateGunReloadImg(bool InVisibility);

	void UpdateEquipWeapon(bool IsEquipMain);
	
protected:
	UPROPERTY()
	TObjectPtr<class UPTHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UPTCharacterStatWidget> CharacterStat;

	UPROPERTY()
	TObjectPtr<class UPTGunInfoWidget> GunInfo;

	UPROPERTY()
	TObjectPtr<class UPTEquipWeaponWidget> EquipWeapon;

	// 	StageTimer in EventGraph
};
