#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/PTGun.h"
#include "PTEquipmentComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	None UMETA(DisplayName="None"),
	Main UMETA(DisplayName="Main"),
	Sub UMETA(DisplayName="Sub"),
	
};

DECLARE_DELEGATE_TwoParams(FOnChangeEquip, EEquipType /* NewEquipType */, APTGun* /* NewEquipment */)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT2_API UPTEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPTEquipmentComponent();

public:
	void Init();
	void ChangeEquipment(EEquipType NewEquipType);

	FORCEINLINE APTGun* GetCurrentGun() const { return CurrentGun; } 

	FOnChangeEquip OnChangeEquip;
	
private:
	void SetEquipment(FString GunDataKey, EEquipType EquipType);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APTGun> MainGunClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APTGun> SubGunClass;

	UPROPERTY(VisibleAnywhere)
	TMap<EEquipType, APTGun*> EquipGuns;

	UPROPERTY(VisibleAnywhere)
	EEquipType CurrentEquipType = EEquipType::None;
	
	UPROPERTY()
	APTGun* CurrentGun;
};
