#include "PTComponent/Character//PTCharacterStatComponent.h"
#include "GameData/PTGameDataSingleton.h"

UPTCharacterStatComponent::UPTCharacterStatComponent()
{
	CurrentLevel = 1;
	MaxHp = 100;
}


void UPTCharacterStatComponent::SetHp(int32 NewHp, int32 NewMaxHp)
{
	MaxHp = NewMaxHp;
	CurrentHp = FMath::Clamp(NewHp, 0.0f, MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}


float UPTCharacterStatComponent::ApplyDamage(int32 InDamage)
{
	const int32 PrevHp = CurrentHp;
	const int32 ActualDamage = FMath::Clamp(InDamage, 0, PrevHp);

	SetHp(PrevHp - ActualDamage, MaxHp);
	if (CurrentHp <= KINDA_SMALL_NUMBER) {
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}
