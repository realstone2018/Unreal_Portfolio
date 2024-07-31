#include "PTProjectile.h"
#include "Components/CapsuleComponent.h"
#include "GameData/PTGameDataSingleton.h"
#include "GameData/PTProjectileData.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/PTCollision.h"
#include "Particles/ParticleSystemComponent.h"

#define ENABLE_DRAW_DEBUG 0

APTProjectile::APTProjectile()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->SetCollisionProfileName(CPROFILE_PTPROJECTILE);
	RootComponent = CapsuleComponent;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetCollisionProfileName(TEXT("NoCollision"));
	ProjectileMesh->SetupAttachment(RootComponent);
	
	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(RootComponent);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->InitialSpeed = 0.f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
}

void APTProjectile::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &APTProjectile::OnOverlapBegin);
}

void APTProjectile::Initialize()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	ProjectileMovementComponent->Activate(true);
	FVector Velocity(2000.f, 0.f, 0.f);
	ProjectileMovementComponent->SetVelocityInLocalSpace(Velocity);

	TrailParticles->Activate();
	
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents(SkeletalMeshComponents);
	
	for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
	{
		SkeletalMeshComponent->SetComponentTickEnabled(true);
	}

	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
}

void APTProjectile::Terminate()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	ProjectileMovementComponent->Deactivate();
	TrailParticles->DeactivateImmediate();
	
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents(SkeletalMeshComponents);
	
	for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
	{
		SkeletalMeshComponent->SetComponentTickEnabled(false);
		if (UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance())
		{
			AnimInstance->StopAllMontages(0.0f);
		}
	}
}

void APTProjectile::Init(FName ProjectileKey)
{
	ProjectileData = UPTGameDataSingleton::Get().GetProjectileData(ProjectileKey);

	CapsuleComponent->SetCapsuleHalfHeight(ProjectileData.Radius);
	CapsuleComponent->SetCapsuleRadius(ProjectileData.Radius);

	ProjectileMovementComponent->InitialSpeed = ProjectileData.MoveSpeed;
	ProjectileMovementComponent->MaxSpeed =  ProjectileData.MoveSpeed;
	
	ProjectileMesh->SetHiddenInGame(false);
	SetActorEnableCollision(true);
	
	CompleteInit = true;
}

void APTProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CompleteInit || GetOwner() == OtherActor)
	{
		return;
	}

	Explosion();
	Dead();
}

void APTProjectile::Explosion()
{
	FVector Location = GetActorLocation();
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	
	TArray<FOverlapResult> OutOverlapResults;
	GetWorld()->OverlapMultiByChannel(OutOverlapResults, Location, FQuat::Identity, CCHANNEL_PTBULLET, FCollisionShape::MakeSphere(ProjectileData.ExplosionRadius), Params);	

	OnExplosion.Execute(OutOverlapResults, Location);

	if (ExplosionParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticles, GetActorLocation(), GetActorRotation());
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(
		GetWorld(),
		Location,
		ExplosionRadius,
		12,
		FColor::Red,
		false,
		3.f
	);
#endif
}

void APTProjectile::Dead()
{
	OnDead.Execute(this);
}
