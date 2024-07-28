#include "Character/PTPlayerController.h"
#include "UI/PTHUDWidget.h"

APTPlayerController::APTPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPTHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));
	if (ABHUDWidgetRef.Class)
	{
		HUDClass = ABHUDWidgetRef.Class;
	}
}

void APTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
