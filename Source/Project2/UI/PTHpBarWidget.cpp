#include "UI/PTHpBarWidget.h"
#include "PTInterface/PTCharactHUDInterface.h"
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
	
	IPTHUDInterface* CharacterWidget = Cast<IPTHUDInterface>(OwningActor);
	if(CharacterWidget)
	{
		CharacterWidget->SetupHpBarWidget(this);		
	}
}

void UPTHpBarWidget::UpdateHpBar(int32 NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	
	if (HpProgressBar) {
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
