#include "PTActor/PTStructure.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PTComponent/PTFactionComponent.h"
#include "PTInterface/PTGameInterface.h"

APTStructure::APTStructure()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Structure Mesh"));
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = MeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(RootComponent);

	FireParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticleComponent"));
	FireParticleComponent->SetupAttachment(RootComponent);

	FactionComponent = CreateDefaultSubobject<UPTFactionComponent>(TEXT("Faction"));
	FactionComponent->SetFaction(EFaction::Player);
	
	static ConstructorHelpers::FClassFinder<AStaticMeshActor> FrameWallClassRef(TEXT("/Game/Project2/Blueprint/Structure/StructureFrame/BP_FrameWall.BP_FrameWall_C"));
	if (FrameWallClassRef.Class)
	{
		FrameWallClass = FrameWallClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AStaticMeshActor> FrameMainStationClassRef(TEXT("/Game/Project2/Blueprint/Structure/StructureFrame/BP_FrameMainStation.BP_FrameMainStation_C"));
	if (FrameMainStationClassRef.Class)
	{
		FrameMainStationClass = FrameMainStationClassRef.Class;
	}
	
	MaxHp = bIsMainStation ? 6000.f : 3000.f;
}

void APTStructure::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHp = MaxHp;

	FireParticleComponent->DeactivateImmediate();
	
	StaticMesh = GetWorld()->SpawnActor<AStaticMeshActor>(
		bIsMainStation ? FrameMainStationClass : FrameWallClass, SceneComponent->GetComponentLocation(), FRotator::ZeroRotator);
	check(StaticMesh);

	StaticMesh->SetActorScale3D(SceneComponent->GetComponentScale());
	StaticMesh->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

float APTStructure::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHp -= DamageAmount;
	
	if (CurrentHp <= 0)
	{
		Destruct();
	}
	
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		FVector DamagedLocation = PointDamageEvent->HitInfo.ImpactPoint;
		FRotator DamagedRotator = PointDamageEvent->HitInfo.ImpactNormal.Rotation();

		if (DamagedEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamagedEffect, DamagedLocation, DamagedRotator);
		}

		if (DamagedSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DamagedSound, GetActorLocation());
		}	
	}
	
	if (CurrentHp < MaxHp * 0.7f)
	{
		FireParticleComponent->Activate();
	}
	
	return result;
}

void APTStructure::Destruct()
{
	if (DestructEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructEffect, SceneComponent->GetComponentLocation(), FRotator::ZeroRotator, FVector(3.f, 3.f, 3.f));
	}

	if (DestructSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DestructSound, GetActorLocation());
	}

	if (bIsMainStation)
	{
		IPTGameInterface* PTGameMode = Cast<IPTGameInterface>(GetWorld()->GetAuthGameMode());
		PTGameMode->OnMainStationDestruct();
	}
	
	// BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// StaticMesh->SetActorHiddenInGame(true);
	// FireEffect->Deactivate();
	StaticMesh->Destroy();
	Destroy();
}
