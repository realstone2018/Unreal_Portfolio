#include "Character/PTCharacterBase.h"
#include "CharacterStat/PTCharacterStatComponent.h"

APTCharacterBase::APTCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	StatComponent = CreateDefaultSubobject<UPTCharacterStatComponent>(TEXT("StatComponent"));
}

void APTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float APTCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	StatComponent->ApplyDamage(result);

	UE_LOG(LogTemp, Display, TEXT("Health: %f"), StatComponent->GetCurrentHp());

	// if (IsDead())
	// {
	// 	Dead();
	// }
	
	return result;
}

