// Fill out your copyright notice in the Description page of Project Settings.

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

	//TODO: 레벨 시스템 적용하면 현재 레벨 넣기 
	SetCharacterLevelStat(1);
}

void UPTPlayerStatComponent::SetCharacterLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UPTGameDataSingleton::Get().CharacterMaxLevel);

	FPTCharacterStat CharacterStat = UPTGameDataSingleton::Get().GetCharacterStat(CurrentLevel);
	SetBaseStat(CharacterStat);
	SetHp(BaseStat.MaxHp, BaseStat.MaxHp);

	//TODO: HP외에도 이것저것 세팅 하기 
	
	check(BaseStat.MaxHp > 0.0f);
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
