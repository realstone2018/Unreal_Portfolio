#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "PTEquipWeaponWidget.generated.h"

UCLASS()
class PROJECT2_API UPTEquipWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void EquipMainWeapon();
	void EquipSubWeapon();
	
private:
	//TODO: 텍스처 이미지 동적 로딩 
	UPROPERTY()
	UImage* MainWeapon;

	UPROPERTY()
	UImage* SubWeapon;

	UPROPERTY()
	UImage* MainSelect;

	UPROPERTY()
	UImage* SubSelect;
	
	static const FLinearColor ShowSelectColor;
	static const FLinearColor HideSelectColor;

	static const FLinearColor SelectWeaponColor;
	static const FLinearColor DeselectWeaponColor;
};
