// Fill out your copyright notice in the Description page of Project Settings.

#include "PTComponent/Character/PTPlayerStatComponent.h"
#include "Components/ActorComponent.h"
#include "GameData/PTCharacterStat.h"
#include "GameData/PTGameDataSingleton.h"

UPTPlayerStatComponent::UPTPlayerStatComponent()
{
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
