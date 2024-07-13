#include "UI/PTHpBarWidget.h"
#include "PTInterface/PTCharacterWidgetInterface.h"
#include "Components/ProgressBar.h"	//UMG모듈에서 지원, Build.cs로 가서 UMG를 추가해줘야한다.


UPTHpBarWidget::UPTHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UPTHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	ensure(HpProgressBar);
	
	IPTCharacterWidgetInterface* CharacterWidget = Cast<IPTCharacterWidgetInterface>(OwningActor);
	if(CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);		
	}
}

void UPTHpBarWidget::UpdateHpBar(int NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	
	if (HpProgressBar) {
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
