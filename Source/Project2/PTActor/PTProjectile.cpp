#include "PTProjectile.h"
#include "Components/CapsuleComponent.h"
#include "GameData/PTGameDataSingleton.h"
#include "GameData/PTProjectileData.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/PTCollision.h"
#include "Particles/ParticleSystemComponent.h"
#include "Manager/PTAssetManager.h"

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

void APTProjectile::Initialize(FName DataKey)
{
	SetData(DataKey);

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	
	ProjectileMovementComponent->Activate(true);
	ProjectileMesh->SetHiddenInGame(false);
	TrailParticles->ActivateSystem(true);
	
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

	CompleteInit = true;
}

void APTProjectile::SetData(FName DataKey)
{
	ProjectileData = UPTGameDataSingleton::Get().GetProjectileData(DataKey);
	LoadResource();
	
	FVector Velocity(ProjectileData.MoveSpeed, 0.f, 0.f);
	ProjectileMovementComponent->SetVelocityInLocalSpace(Velocity);
	
	CapsuleComponent->SetCapsuleHalfHeight(ProjectileData.Radius);
	CapsuleComponent->SetCapsuleRadius(ProjectileData.Radius);
}

void APTProjectile::LoadResource()
{
	UE_LOG(LogTemp, Log, TEXT("[%f] APTProjectile::LoadResource() - Try Load"), GetWorld()->GetTimeSeconds());
	
	UPTAssetManager& AssetManager = UPTAssetManager::Get();

	UStaticMesh* Mesh = AssetManager.GetdMeshAsset<UStaticMesh>(ProjectileData.StaticMesh);
	ProjectileMesh->SetStaticMesh(Mesh);		

	UParticleSystem* TrailEffect = AssetManager.GetFXAsset<UParticleSystem>(ProjectileData.TrailEffect);
	TrailParticles->SetTemplate(TrailEffect);

	UParticleSystem* ExplosionEffect = AssetManager.GetFXAsset<UParticleSystem>(ProjectileData.ExplosionEffect);
	ExplosionParticle = ExplosionEffect;
	
	USoundWave* Sound = AssetManager.GetSFXAsset<USoundWave>(ProjectileData.ExplosionSound);
	ExplosionSound = Sound;
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

void APTProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("APTProjectile::OnOverlapBegin() - CompleteInit: %s    OtherActor: %s"), CompleteInit ? TEXT("True") : TEXT("False"), *OtherActor->GetName());

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
	TArray<FOverlapResult> OutOverlapResults;
	GetWorld()->OverlapMultiByChannel(OutOverlapResults, Location, FQuat::Identity, CCHANNEL_PTBULLET, FCollisionShape::MakeSphere(ProjectileData.ExplosionRadius), Params);	

	OnExplosion.Execute(OutOverlapResults, Location);

	if (ExplosionParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, GetActorLocation(), GetActorRotation());
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