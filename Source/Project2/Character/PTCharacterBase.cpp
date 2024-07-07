#include "SimpleShooterGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "Character/PTCharacterBase.h"
#include "PTComponent/Character/PTCharacterStatComponent.h"
#include "PTComponent/Character/PTCharacterMoveComponent.h"
#include "PTComponent/PTFactionComponent.h"
#include "UI/PTHpBarWidget.h"
#include "UI/PTUserWidget.h"

APTCharacterBase::APTCharacterBase()
{
	UE_LOG(LogTemp, Display, TEXT("APTCharacterBase::APTCharacterBase()"));

	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	
	MoveComponent = Cast<UPTCharacterMoveComponent>(GetMovementComponent());
	FactionComponent = CreateDefaultSubobject<UPTFactionComponent>(TEXT("Faction"));

	HpBar = CreateDefaultSubobject<UPTWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	HpBar->SetHiddenInGame(true);

	static ConstructorHelpers::FClassFinder<UPTUserWidget> HpBarWidgetRef(TEXT("/Game/Project2/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBarWidgetClass = HpBarWidgetRef.Class;
	}
}

void APTCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HpBar && HpBarWidgetClass && GetShouldDisplayHpBar())
	{
		HpBar->SetWidgetClass(HpBarWidgetClass); 
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.f, 15.f)); //위젯이 담길 사이즈 (패널의 크기?)
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision); //불필요한 충돌처리 비활성화

		HpBar->SetHiddenInGame(false);
	}
}

UPTCharacterStatComponent* APTCharacterBase::GetStatComponent()
{
	UE_LOG(LogTemp, Display, TEXT("APTCharacterBase::GetStatComponent()"));

	return nullptr;
}

void APTCharacterBase::SetupCharacterWidget(UPTUserWidget* InUserWidget)
{
	UPTHpBarWidget* HpBarWidget = Cast<UPTHpBarWidget>(InUserWidget);
	UPTCharacterStatComponent* Stat = GetStatComponent();
	if (Stat == nullptr) {
		return;
	}
	
	if (HpBarWidget) {
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UPTHpBarWidget::UpdateHpBar);

		// HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		//
		// Stat->OnHpChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateHpBar);
		// //14-5
		// Stat->OnStatChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateStat);

		HpBar->SetHiddenInGame(false);
	}	
}

void APTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APTCharacterBase::Evation()
{
}

void APTCharacterBase::OnNotifyAttack()
{
	UE_LOG(LogTemp, Display, TEXT("APTCharacterBase::OnNotifyAttack()"));

}

float APTCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	GetStatComponent()->ApplyDamage(result);
	
	if (IsDead())
	{
		Dead();

		APTCharacterBase* CauserCharacter = Cast<APTCharacterBase>(DamageCauser);
		if (IsValid(CauserCharacter))
		{
			CauserCharacter->Kill(this);
		}
	}
	
	return result;
}

bool APTCharacterBase::IsDead()
{
	return GetStatComponent()->GetCurrentHp() <= 0;
}

void APTCharacterBase::Kill(AActor* victim)
{
}

void APTCharacterBase::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HpBar->SetHiddenInGame(true);
}

