#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PTInterface/PTFactionInterface.h"
#include "PTStructure.generated.h"

UCLASS()
class PROJECT2_API APTStructure : public AActor, public IPTFactionInterface
{
	GENERATED_BODY()
	
public:	
	APTStructure();

protected:
	virtual void BeginPlay() override;

public:
	virtual EFaction GetFaction() override { return EFaction::Player; }

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	uint8 GetbIsMainStation() { return bIsMainStation; }

protected:
	virtual void Destruct();

#pragma region Component
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Component")
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	TObjectPtr<class UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta=(AllowPrivateAccess))
	TObjectPtr<class USceneComponent> SceneComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AStaticMeshActor> FrameWallClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AStaticMeshActor> FrameMainStationClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AStaticMeshActor> StaticMesh; 

#pragma endregion
	
protected:
	UPROPERTY()
	int32 MaxHp;
	
	UPROPERTY()
	int32 CurrentHp;	// 방어력없음, Hp로만 내구성 구현하기

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	uint8 bIsMainStation : 1;

#pragma region Effect, Sound
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> DamagedEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DamagedSound;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystemComponent> FireParticleComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> DestructEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DestructSound;

#pragma endregion
	
};
