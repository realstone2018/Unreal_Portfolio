#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PTCharacterMoveComponent.generated.h"

UCLASS()
class PROJECT2_API UPTCharacterMoveComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPTCharacterMoveComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveInput(FVector Direction, float MoveSpeed, float MoveDuration);
	
	void MoveToLocation(FVector InTargetLocation, float Duration);
	void MoveToDirection(FVector Direction, float Distance, float Duration);

	uint8 GetIsMoving() const { return bIsMoving; }

	UPROPERTY(EditAnywhere, Category="PTMove")
	float MoveScale;
	
private:
	uint8 bIsMoving : 1;
	uint8 bIsMovingUseInput : 1;
	
	FVector StartLocation;
	FVector TargetLocation;
	FVector MoveDirection;
	
	float MoveDuration;
	float ElapsedTime;
};
