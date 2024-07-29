#include "PTComponent/Character/PTCharacterMoveComponent.h"

UPTCharacterMoveComponent::UPTCharacterMoveComponent()
{
	MoveDuration = 1.0f;
	ElapsedTime = 0.0f;
	MoveScale = 1.f;
}

void UPTCharacterMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsMoving)
	{
		ElapsedTime += DeltaTime;
		float Alpha = FMath::Min(ElapsedTime / MoveDuration, 1.0f);
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
		
		GetOwner()->SetActorLocation(NewLocation);
		
		if (Alpha >= 1.0f)
		{
			bIsMoving = false;
			StopMovementImmediately();
		}
	}
	else if (bIsMovingUseInput)
	{
		ElapsedTime += DeltaTime;
		float Alpha = FMath::Min(ElapsedTime / MoveDuration, 1.0f);	

		AddInputVector(MoveDirection * MoveScale, false);
		
		if (Alpha >= 1.0f)
		{
			bIsMovingUseInput = false;
		}
	}
}

void UPTCharacterMoveComponent::MoveInput(FVector Direction, float MoveSpeed, float InMoveDuration)
{
	ElapsedTime = 0.0f;
	MoveDuration = InMoveDuration;
	MoveDirection = Direction;
	bIsMovingUseInput = true;
}

void UPTCharacterMoveComponent::MoveToLocation(FVector InTargetLocation, float Duration) 
{
	ElapsedTime = 0.0f;
	StartLocation = GetActorLocation();
	MoveDuration = Duration;
	TargetLocation = InTargetLocation; 
	bIsMoving = true;
}

//TODO: CurrentMove로 Move 관리 필요 
//TODO: Direction을 FRotator, FVector 뭐로 받아야할지 확인 필요
void UPTCharacterMoveComponent::MoveToDirection(FVector Direction, float Distance, float Duration)
{
	ElapsedTime = 0.0f;
	StartLocation = GetActorLocation();
	TargetLocation = GetActorLocation() + (Direction * Distance);
	//UE_LOG(LogTemp, Display, TEXT("Current: %s   Target: %s"), *GetActorLocation().ToString(), *TargetLocation.ToString());

	MoveDuration = Duration;

	bIsMoving = true;
}
