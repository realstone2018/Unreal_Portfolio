#include "PTActor/PTStructure.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "PTComponent/PTFactionComponent.h"

APTStructure::APTStructure()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Structure Mesh"));
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = MeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(RootComponent);

	DamagedParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Particle"));
	DamagedParticles->SetupAttachment(RootComponent);
	
	FactionComponent = CreateDefaultSubobject<UPTFactionComponent>(TEXT("Faction"));
	FactionComponent->SetFaction(EFaction::Ally);
	
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
	
	MaxHp = 100.f;
}

void APTStructure::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHp = MaxHp;

	DamagedParticles->DeactivateImmediate();

	AStaticMeshActor* StaticMeshActor = GetWorld()->SpawnActor<AStaticMeshActor>(
		bIsMainStation ? FrameMainStationClass : FrameWallClass, SceneComponent->GetComponentLocation(), FRotator::ZeroRotator);
	check(StaticMeshActor);

	UE_LOG(LogTemp, Display, TEXT("SceneComponent->GetComponentLocation(): %s    StaticMeshActor->GetActorLocation(): %s"), *SceneComponent->GetComponentLocation().ToString(), *StaticMeshActor->GetActorLocation().ToString());
	
	StaticMeshActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	UE_LOG(LogTemp, Display, TEXT("SceneComponent->GetRelativeLocation(): %s    StaticMeshActor->GetActorLocation(): %s"), *SceneComponent->GetRelativeLocation().ToString(), *StaticMeshActor->GetActorLocation().ToString());
	UE_LOG(LogTemp, Display, TEXT("SceneComponent->GetComponentLocation(): %s    StaticMeshActor->GetActorLocation(): %s"), *SceneComponent->GetComponentLocation().ToString(), *StaticMeshActor->GetActorLocation().ToString());

}

float APTStructure::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHp -= DamageAmount;
	if (CurrentHp <= 0)
	{
		Destructed();
	}
	else if (CurrentHp < MaxHp / 3.f)
	{
		DamagedParticles->Activate();
	}
	
	return result;
}

void APTStructure::Destructed()
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}
