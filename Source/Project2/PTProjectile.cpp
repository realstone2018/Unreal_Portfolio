#include "PTProjectile.h"

#include "Character/PTMonster.h"
#include "GameFramework/ProjectileMovementComponent.h" //
#include "GameFramework/DamageType.h" // for UDamageType::StaticClass()
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
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->InitialSpeed = 2000.f;

}

void APTProjectile::BeginPlay()
{
	UE_LOG(LogTemp, Display, TEXT("APTProjectile::BeginPlay()"));
	
	Super::BeginPlay();

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &APTProjectile::OnOverlapBegin);
	
	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
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
	UE_LOG(LogTemp, Display, TEXT("APTProjectile::OnOverlapBegin() - OtherActor: %s"), *OtherActor->GetName());

	if (!CompleteInit || GetOwner() == OtherActor)
	{
		return;
	}
	
	if (GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("APTProjectile::OnOverlapBegin() - MyOwner is Null"));

		Destroy();
		return;
	}

	Explosion();

	//TODO: Projectile 오브젝트 풀링 준비
	ProjectileMesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	
	Destroy();
}

void APTProjectile::Explosion()
{
	FVector Location = GetActorLocation();
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	
	TArray<FOverlapResult> OutOverlapResults;
	GetWorld()->OverlapMultiByChannel(OutOverlapResults, Location, FQuat::Identity, CCHANNEL_PTBULLET, FCollisionShape::MakeSphere(ExplosionRadius), Params);	

	OnExplosion.Execute(GetOwner(), OutOverlapResults);

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
