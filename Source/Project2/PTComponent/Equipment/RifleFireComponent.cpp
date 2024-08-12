#include "PTComponent/Equipment//RifleFireComponent.h"
#include "Physics/PTCollision.h"

#define ENABLE_DRAW_DEBUG 0

void URifleFireComponent::FireProcess(FVector SpawnPoint, FVector ViewLocation, FRotator ViewDirection, float Range, int32 Damage)
{
	FHitResult HitResult;
	FVector End = ViewLocation + ViewDirection.Vector() * Range;
	uint8 bIsHit = FireLineTracing(HitResult, ViewLocation, End);
	if (!bIsHit)
	{
		return;
	}

	FVector DamageDirection = -ViewDirection.Vector();
	Gun->DamageToHitResult(HitResult, Damage, DamageDirection);
	
#if ENABLE_DRAW_DEBUG
	DrawDebugPoint(GetWorld(), HitResult.Location, 10, FColor::Red, true);
#endif
}

uint8 URifleFireComponent::FireLineTracing(FHitResult& HitResult, FVector SpawnPoint, FVector End)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(Gun->GetOwner());

#if ENABLE_DRAW_DEBUG
	DrawDebugLine(GetWorld(), SpawnPoint, End, FColor::Red, true, 5.f);
#endif

	return GetWorld()->LineTraceSingleByChannel(HitResult, SpawnPoint, End, CCHANNEL_PTBULLET, Params);
}