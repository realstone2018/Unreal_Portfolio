#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunFireComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FOnHitTracing, FHitResult /*hit*/, FVector /*ShotDirection*/)

UCLASS(Abstract)
class PROJECT2_API UGunFireComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* InRifle);
	virtual void FireProcess(FVector SpawnPoint, float Range, int32 Damage, FString ProjectileName) PURE_VIRTUAL(UGunFireComponent::FireProcess, ;);

	FOnHitTracing OnHitTracing;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> Gun;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class APTCharacterBase> GunOwner;
};
