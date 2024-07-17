#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PTStructure.generated.h"

UCLASS()
class PROJECT2_API APTStructure : public AActor
{
	GENERATED_BODY()
	
public:	
	APTStructure();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UPTFactionComponent> FactionComponent;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destructed();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Component")
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	TObjectPtr<class UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UParticleSystemComponent> DamagedParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta=(AllowPrivateAccess))
	TObjectPtr<class USceneComponent> SceneComponent;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AStaticMeshActor> FrameWallClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AStaticMeshActor> FrameMainStationClass;
	
	UPROPERTY()
	int MaxHp;
	
	UPROPERTY()
	int CurrentHp;	// 방어력없음, Hp로만 내구성 구현하기

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Structure", meta=(AllowPrivateAccess))
	uint8 bIsMainStation : 1;
};
