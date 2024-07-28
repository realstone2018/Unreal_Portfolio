#include "Character/PTCharacterBase.h"
#include "SimpleShooterGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "PTComponent/Character/PTCharacterStatComponent.h"
#include "PTComponent/Character/PTCharacterMoveComponent.h"
#include "PTComponent/PTFactionComponent.h"
#include "UI/PTWidgetComponent.h"
#include "UI/PTHpBarWidget.h"
#include "UI/PTUserWidget.h"

APTCharacterBase::APTCharacterBase()
{
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	
	MoveComponent = Cast<UPTCharacterMoveComponent>(ACharacter::GetMovementComponent());
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
		HpBar->SetDrawSize(FVector2D(150.f, 15.f)); 
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		HpBar->SetHiddenInGame(false);
	}
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

void APTCharacterBase::Dead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HpBar->SetHiddenInGame(true);
}

void APTCharacterBase::SetupHpBarWidget(UPTUserWidget* InUserWidget)
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
		HpBar->SetHiddenInGame(false);
	}	
}