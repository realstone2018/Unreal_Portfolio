#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class PROJECT2_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter();

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	
protected:
	virtual void BeginPlay() override;

#pragma region Input
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//BTTask 노드에서 호출하기 위해 public으로 변경
	void ShootCalllback();
	
protected:
	void SetCharacterControl();

	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	TObjectPtr<class UInputAction> MoveAction;
	TObjectPtr<class UInputAction> LookAction;
	TObjectPtr<class UInputAction> JumpAction;
	TObjectPtr<class UInputAction> ShootAction;

	void MoveCallback(const FInputActionValue& Value);
	void LookCallback(const FInputActionValue& Value);

private:
	//TODO: 옵션에서 감도 설정할 수 있게
	UPROPERTY(EditAnywhere)
	float RotationRate = 60;

#pragma endregion


#pragma region Battle
public:	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

private:
	void Dead();
	
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;

#pragma endregion

	
private:
	//인스턴스를 저장하는게 아닌, 클래스 자체를 저장하기 때문에 포인터가 붙지 않는다.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> GunClass;

	//스폰한 인스턴스를 저장하기에 포인터
	UPROPERTY()
	AGun* Gun;
	

};
