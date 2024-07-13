#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PTPlayerController.generated.h"

UCLASS()
class PROJECT2_API APTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APTPlayerController();

	virtual void BeginPlay() override;
	
protected: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UPTHUDWidget> HUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UPTHUDWidget> HUD;
};
