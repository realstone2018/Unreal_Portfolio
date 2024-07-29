#include "PTComponent/Character/PTPlayerStatComponent.h"
#include "Components/ActorComponent.h"
#include "GameData/PTCharacterStat.h"
#include "GameData/PTGameDataSingleton.h"

UPTPlayerStatComponent::UPTPlayerStatComponent()
{	 
	bWantsInitializeComponent = true;
}

void UPTPlayerStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetCharacterLevelStat(1);
}

void UPTPlayerStatComponent::SetCharacterLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UPTGameDataSingleton::Get().CharacterMaxLevel);

	FPTCharacterStat CharacterStat = UPTGameDataSingleton::Get().GetCharacterStat(CurrentLevel);
	SetBaseStat(CharacterStat);
	SetHp(BaseStat.MaxHp, BaseStat.MaxHp);
}

void UPTPlayerStatComponent::SetBaseStat(const FPTCharacterStat& InBaseStat)
{
	BaseStat = InBaseStat;
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UPTPlayerStatComponent::SetModifierStat(const FPTCharacterStat& InModifierStat)
{
	ModifierStat = InModifierStat;
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

void UPTPlayerStatComponent::AddBaseStat(const FPTCharacterStat& InAddBaseStat)
{
	BaseStat = BaseStat + InAddBaseStat;
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}
