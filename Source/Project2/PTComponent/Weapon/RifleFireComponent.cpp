#include "PTComponent/Weapon/RifleFireComponent.h"
#include "Engine/DamageEvents.h"

//TODO: OwnerController 매개변수에서 제거 가능한지 알아보기
// Gun을 넘길까?
// Interface?
void URifleFireComponent::FireProcess(FVector SpawnPoint, float Range, int Damage)
{
	UE_LOG(LogTemp, Display, TEXT("URifleFireComponent::FireProcess"));

	FHitResult HitResult;
	FVector ShotDirection;
	bool IsHit = GunTrace(HitResult, ShotDirection, SpawnPoint, Range);
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

	DrawDebugPoint(GetWorld(), HitResult.Location, 10, FColor::Red, true);
}

bool URifleFireComponent::GunTrace(FHitResult& HitResult, FVector& ShotDirection, FVector SpawnPoint, float Range)
{
	UE_LOG(LogTemp, Display, TEXT("URifleFireComponent::GunTrace"));
	
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

	DrawDebugLine(GetWorld(), OutLocation, End, FColor::Red, true, 5.f);

	//어딘가에 부딪쳐야만 True반환
	return GetWorld()->LineTraceSingleByChannel(HitResult, OutLocation, End, ECC_GameTraceChannel1, Params);
}
