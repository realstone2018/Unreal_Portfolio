#include "Character/CharacterStat/PTCharacterStatComponent.h"
#include "GameData/PTGameDataSingleton.h"

UPTCharacterStatComponent::UPTCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.0f;
	bWantsInitializeComponent = true;
}

void UPTCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UPTCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UPTGameDataSingleton::Get().CharacterMaxLevel);

	FPTCharacterStat CharacterStat = UPTGameDataSingleton::Get().GetCharacterStat(CurrentLevel);
	SetBaseStat(CharacterStat);
	
	check(BaseStat.MaxHp > 0.0f);
}

void UPTCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}


float UPTCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, PrevHp);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER) {
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}
