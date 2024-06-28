#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/PTCharacterStat.h"
#include "PTCharacterStatWidget.generated.h"

UCLASS()
class PROJECT2_API UPTCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void UpdateStat(const FPTCharacterStat& BaseStat, const FPTCharacterStat& ModifierStat);

private:
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;

	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;
};
