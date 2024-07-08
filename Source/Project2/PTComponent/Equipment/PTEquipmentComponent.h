#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/Gun.h"
#include "PTEquipmentComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	None UMETA(DisplayName="None"),
	Main UMETA(DisplayName="Main"),
	Sub UMETA(DisplayName="Sub"),
	
};

DECLARE_DELEGATE_OneParam(FOnChangeEquip, AGun* /* NewEquipment */)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT2_API UPTEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPTEquipmentComponent();

public:
	void Init();
	void ChangeEquipment(EEquipType NewEquipType);

	FORCEINLINE AGun* GetCurrentGun() const { return CurrentGun; } 

	FOnChangeEquip OnChangeEquip;
	
private:	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> MainGunClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> SubGunClass;

	UPROPERTY(VisibleAnywhere)
	TMap<EEquipType, AGun*> EquipGuns;

	UPROPERTY(VisibleAnywhere)
	EEquipType CurrentEquipType = EEquipType::None;
	
	UPROPERTY()
	AGun* CurrentGun;
};
