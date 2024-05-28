// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "PTCameraShakePattern.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT2_API UPTCameraShakePattern : public UCameraShakePattern
{
	GENERATED_BODY()

public:
	virtual void StartShakePatternImpl(const FCameraShakePatternStartParams& Params) override;
	virtual void UpdateShakePatternImpl(const FCameraShakePatternUpdateParams& Params, FCameraShakePatternUpdateResult& OutResult) override;

private:
	FRandomStream RandomStream;
};
