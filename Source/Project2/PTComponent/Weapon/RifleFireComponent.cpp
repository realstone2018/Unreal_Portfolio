#include "PTComponent/Weapon/RifleFireComponent.h"
#include "Engine/DamageEvents.h"

void URifleFireComponent::Init(AActor* InRifle)
{
	UE_LOG(LogTemp, Display, TEXT("URifleFireComponent::Init"));

	Gun = InRifle;
	GunOwner = Cast<APawn>(Gun->GetOwner());
}

//TODO: OwnerController 매개변수에서 제거 가능한지 알아보기
// Gun을 넘길까?
// Interface?
bool URifleFireComponent::FireProcess(float Range, int Damage)
{
	UE_LOG(LogTemp, Display, TEXT("URifleFireComponent::FireProcess"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection, Range);
	if (!bSuccess)
	{
		return false;
	}

	OnHitTracing.Execute(Hit, ShotDirection);
	
	AActor* HitActor = Hit.GetActor();
	if (HitActor != nullptr)
	{
		FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
		HitActor->TakeDamage(Damage, DamageEvent, GunOwner->GetController(), Gun);

		return true;
	}
	
	DrawDebugPoint(GetWorld(), Hit.Location, 10, FColor::Red, true);

	return false;
}

bool URifleFireComponent::GunTrace(FHitResult& Hit, FVector& ShotDirection, float Range)
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
	return GetWorld()->LineTraceSingleByChannel(Hit, OutLocation, End, ECC_GameTraceChannel1, Params);
}
