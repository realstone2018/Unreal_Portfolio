#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PTFactionComponent.generated.h"

UENUM(BlueprintType)
enum class EFaction : uint8
{
	None UMETA(DisplayName="None"),
	Player UMETA(DisplayName="Player"),
	Monster UMETA(DisplayName=Monster),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT2_API UPTFactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetFaction(EFaction InFaction) { Faction = InFaction; }
	FORCEINLINE EFaction GetFaction() const { return Faction; }

	FORCEINLINE bool IsNoneFaction() const { return Faction == EFaction::None; }
	FORCEINLINE bool CompareFaction(const UPTFactionComponent* Other) const { return GetFaction() ==  Other->GetFaction(); }
	FORCEINLINE bool CompareFaction(EFaction OtherFaction) const { return GetFaction() ==  OtherFaction; }
	
private:
	EFaction Faction = EFaction::None;

};
