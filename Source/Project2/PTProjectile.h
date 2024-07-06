#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/PTCharacterBase.h"
#include "PTProjectile.generated.h"

DECLARE_DELEGATE_TwoParams(FOnHitEnemies, AActor* /*Owner*/, const TArray<AActor*>& /*targets*/)

UCLASS()
class PROJECT2_API APTProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	APTProjectile();

	void Init(AActor* Owner);
	
	FOnHitEnemies OnHitArea;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	bool CompleteInit = false;
	
	UPROPERTY(EditAnywhere)
	float ExplosionRadius = 10.0f;
	
private:
	UPROPERTY(EditDefaultsOnly, category = "Component")
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, category = "Component")
	TObjectPtr<class UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UParticleSystemComponent> TrailParticles;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

private:
	UPROPERTY(EditAnywhere, Category = "EffectAndSound")
	class USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "EffectAndSound")
	class UParticleSystem* ExplosionParticles;

	UPROPERTY(EditAnywhere, Category = "EffectAndSound")
	class USoundBase* ExplosionSound;
 
	UPROPERTY(EditAnywhere, Category = "EffectAndSound")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;

public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
