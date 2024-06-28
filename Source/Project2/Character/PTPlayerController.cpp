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

	// 마우스를 뷰포트 안으로 
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	//TODO: 블루프린트로 대체 (15강) 
	HUD = CreateWidget<UPTHUDWidget>(this, HUDClass);
	if (HUD)
	{
		HUD->AddToViewport();
	}
}
