#include "UI/PTWidgetComponent.h"
#include "UI/PTUserWidget.h"

void UPTWidgetComponent::InitWidget()
{
	Super::InitWidget();
	
	UPTUserWidget* ABUserWidget = Cast<UPTUserWidget>(GetWidget());
	if (ABUserWidget) {
		ABUserWidget->SetOwningActor(GetOwner());
	}
}
