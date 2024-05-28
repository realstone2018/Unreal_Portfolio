// Fill out your copyright notice in the Description page of Project Settings.


#include "PTCameraShakePattern.h"

void UPTCameraShakePattern::StartShakePatternImpl(const FCameraShakePatternStartParams& Params)
{
	Super::StartShakePattern(Params);
	RandomStream.Initialize(FMath::Rand());
}

void UPTCameraShakePattern::UpdateShakePatternImpl(const FCameraShakePatternUpdateParams& Params,
	FCameraShakePatternUpdateResult& OutResult)
{
	// Params.POV.Rotation.Pitch += FMath::Sin(RandomStream.FRand() * 25.0f) * 5.0f;
	//
	// Params.POV.Rotation.Yaw += 0.f;
	//
	// Params.POV.Rotation.Pitch = FMath::RandRange(-5.0f, 5.0f) * 1.f;
	// Params.POV.Rotation.Yaw += FMath::RandRange(-2.0f, 2.0f) * 1.f;
}
