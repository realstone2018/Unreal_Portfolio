#include "UI/PTWidgetComponent.h"
#include "UI/PTUserWidget.h"

void UPTWidgetComponent::InitWidget()
{
	Super::InitWidget();	//Super에서 UI위젯의 인스턴스가 생성됨

	//Super이후에 인스턴스에 접근 가능
	UPTUserWidget* ABUserWidget = Cast<UPTUserWidget>(GetWidget());
	if (ABUserWidget) {
		ABUserWidget->SetOwningActor(GetOwner());	//UABUserWidget는 컴포넌트라서, GetOwner()로 소유자를 가져올 수있다.
	}
}
