#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PTUserWidget.generated.h"

UCLASS()
class PROJECT2_API UPTUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }

protected:
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor;
	
};
