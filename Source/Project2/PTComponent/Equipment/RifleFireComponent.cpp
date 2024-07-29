#include "PTComponent/Equipment//RifleFireComponent.h"
#include "Engine/DamageEvents.h"

#define ENABLE_DRAW_DEBUG 0

//TODO: OwnerController 매개변수에서 제거 가능한지 알아보기
// Gun을 넘길까?
// Interface?
void URifleFireComponent::FireProcess(FVector SpawnPoint, float Range, int32 Damage)
{
	FHitResult HitResult;
	FVector ShotDirection;
	uint8 IsHit = GunTrace(HitResult, ShotDirection, SpawnPoint, Range);
	if (!IsHit)
	{
		return;
	}

	OnHitTracing.Execute(HitResult, ShotDirection);

	AActor* HitActor = HitResult.GetActor();
	if (HitActor != nullptr)
	{
		FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
		HitActor->TakeDamage(Damage, DamageEvent, GunOwner->GetController(), Gun);;
	}

#if ENABLE_DRAW_DEBUG
	DrawDebugPoint(GetWorld(), HitResult.Location, 10, FColor::Red, true);
#endif
}

uint8 URifleFireComponent::GunTrace(FHitResult& HitResult, FVector& ShotDirection, FVector SpawnPoint, float Range)
{
	// 폰이 보고있는 시야의 시작 위치와 회전방향을 가져온다. (카메라가 붙어있는 경우 카메라 베이스로, 없는 경우는 모르겠다.)
	FVector OutLocation;
	FRotator OutRotation;
	GunOwner->GetController()->GetPlayerViewPoint(OutLocation, OutRotation);
	
	//컨트롤러를 가져와, 컨트롤러를 통해 뷰포트의 Location과 Rotation을 가져온다.
	// 폰이 보고있는 시야의 시작 위치와 회전방향을 가져온다. (카메라가 붙어있는 경우 카메라 베이스로, 없는 경우는 모르겠다.)
	FVector End = OutLocation + OutRotation.Vector() * Range;
	ShotDirection =	-OutRotation.Vector();
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(GunOwner);

#if ENABLE_DRAW_DEBUG
	DrawDebugLine(GetWorld(), OutLocation, End, FColor::Red, true, 5.f);
#endif
	
	//어딘가에 부딪쳐야만 True반환
	return GetWorld()->LineTraceSingleByChannel(HitResult, OutLocation, End, ECC_GameTraceChannel1, Params);
}
