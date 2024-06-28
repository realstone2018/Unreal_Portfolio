#include "PTComponent/Character//PTCharacterStatComponent.h"
#include "GameData/PTGameDataSingleton.h"

UPTCharacterStatComponent::UPTCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.0f;
	MaxHp = 100;
}


void UPTCharacterStatComponent::SetHp(int NewHp, int NewMaxHp)
{
	MaxHp = NewMaxHp;
	CurrentHp = FMath::Clamp(NewHp, 0.0f, MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}


float UPTCharacterStatComponent::ApplyDamage(int InDamage)
{
	const int PrevHp = CurrentHp;
	const int ActualDamage = FMath::Clamp(InDamage, 0, PrevHp);

	SetHp(PrevHp - ActualDamage, MaxHp);
	if (CurrentHp <= KINDA_SMALL_NUMBER) {
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}
