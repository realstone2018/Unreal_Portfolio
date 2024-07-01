// Fill out your copyright notice in the Description page of Project Settings.


#include "PTComponent/Weapon/LauncherFireComponent.h"

ULauncherFireComponent::ULauncherFireComponent()
{
}

bool ULauncherFireComponent::FireProcess(float Range, int Damage)
{
	return false;
}

bool ULauncherFireComponent::GunTrace(FHitResult& Hit, FVector& ShotDirection, float Range)
{
	return false;
}

