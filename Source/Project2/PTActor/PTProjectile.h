#pragma once

#include "CoreMinimal.h"
#include "GameData/PTProjectileData.h"
#include "GameFramework/Actor.h"
#include "PTInterface/PTPullingObjectInterface.h"
#include "PTProjectile.generated.h"

DECLARE_DELEGATE_TwoParams(FOnExplosionDelegate, const TArray<FOverlapResult>& /*targets*/, FVector /* Location */);
DECLARE_DELEGATE_OneParam(FOnDeadDelegate, AActor* /*this*/);

UCLASS()
class PROJECT2_API APTProjectile : public AActor, public IPTPullingObjectInterface
{
	GENERATED_BODY()
	
public:	
	APTProjectile();

	FOnExplosionDelegate OnExplosion;
	FOnDeadDelegate OnDead;
	
protected:
	virtual void BeginPlay() override;

	virtual void Initialize() override;
	virtual void Terminate() override;
	
public:
	void SetData(FName DataKey);

	void LoadResource();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void Explosion();

	void Dead();

private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPTProjectileData ProjectileData;
	
	UPROPERTY(EditDefaultsOnly, category = "Component")
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, category = "Component")
	TObjectPtr<class UStaticMeshComponent> ProjectileMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY()
	uint8 CompleteInit : 1;

#pragma region Effect, Sound
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UParticleSystemComponent> TrailParticles;

	UPROPERTY(EditAnywhere, Category = "EffectAndSound")
	TObjectPtr<class USoundBase> LaunchSound;

	UPROPERTY(EditAnywhere, Category = "EffectAndSound")
	TObjectPtr<class UParticleSystem> ExplosionParticles;

	UPROPERTY(EditAnywhere, Category = "EffectAndSound")
	TObjectPtr<class USoundBase> ExplosionSound;

#pragma endregion
	
};
