#include "PTProjectile.h"

#include "Character/PTMonster.h"
#include "GameFramework/ProjectileMovementComponent.h" //
#include "Kismet/GameplayStatics.h" // UGameplayStatics::ApplyDamage()
#include "Particles/ParticleSystemComponent.h"
#include "Physics/PTCollision.h"
#include "Components/CapsuleComponent.h"

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
	//ProjectileMovementComponent->bUpdateOnlyIfRendered = true;
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->MaxSpeed = 2000.f;
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
	
	// 애니메이션 활성화
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
	
	// 애니메이션 비활성화
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

void APTProjectile::Init(AActor* GunOwner)
{
	SetOwner(GunOwner);

	//TODO: Projectile 오브젝트 풀링 준비
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
	
	if (GetOwner() == nullptr)
	{
		Dead();
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
	GetWorld()->OverlapMultiByChannel(OutOverlapResults, Location, FQuat::Identity, CCHANNEL_PTBULLET, FCollisionShape::MakeSphere(ExplosionRadius), Params);	

	OnExplosion.Execute(GetOwner(), OutOverlapResults, Location);

	if (ExplosionParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticles, GetActorLocation(), GetActorRotation());
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}

	if (HitCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
	}
	
	DrawDebugSphere(
		GetWorld(),
		Location,
		ExplosionRadius,
		12,
		FColor::Red,
		false,
		3.f
	);
}

void APTProjectile::Dead()
{
	OnDead.Execute(this);
}
