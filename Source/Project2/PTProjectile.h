#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/PTCharacterBase.h"
#include "Interface/PTPullingObjectInterface.h"
#include "PTProjectile.generated.h"

DECLARE_DELEGATE_TwoParams(FOnExplosionDelegate, AActor* /*Owner*/, const TArray<FOverlapResult>& /*targets*/)
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
	void Init(AActor* Owner);
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void Explosion();

	void Dead();
	
private:
	UPROPERTY()
	bool CompleteInit = false;

	//TODO: 데이터로 옮기기 
	UPROPERTY(EditAnywhere)
	float ExplosionRadius = 200.0f;
	
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
	TObjectPtr<class USoundBase> LaunchSound;

	UPROPERTY(EditAnywhere, Category = "EffectAndSound")
	TObjectPtr<class UParticleSystem> ExplosionParticles;

	UPROPERTY(EditAnywhere, Category = "EffectAndSound")
	TObjectPtr<class USoundBase> ExplosionSound;
 
	UPROPERTY(EditAnywhere, Category = "EffectAndSound")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;
};
