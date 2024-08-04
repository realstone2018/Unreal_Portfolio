#include "PTComponent/Equipment/PTEquipmentComponent.h"
#include "GameData/PTGameDataSingleton.h"
#include "GameFramework/Character.h"

UPTEquipmentComponent::UPTEquipmentComponent()
{
	static ConstructorHelpers::FClassFinder<APTGun> MainGunRef(TEXT("/Game/Project2/Blueprint/Gun/BP_Rifle.BP_Rifle_C"));
	if (MainGunRef.Class)
	{
		MainGunClass = MainGunRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APTGun> SubGunRef(TEXT("/Game/Project2/Blueprint/Gun/BP_Launcher.BP_Launcher_C"));
	if (SubGunRef.Class)
	{
		SubGunClass = SubGunRef.Class;
	}
}

void UPTEquipmentComponent::Init()
{
	SetEquipment(TEXT("BaseRifle"), EEquipType::Main);
	SetEquipment(TEXT("BaseLauncher"), EEquipType::Sub);
	
	ChangeEquipment(EEquipType::Main);
}

void UPTEquipmentComponent::ChangeEquipment(EEquipType NewEquipType)
{
	if (!EquipGuns.Contains(NewEquipType))
	{
		return;
	}

	if (CurrentEquipType == NewEquipType)
	{
		return;
	}

	CurrentEquipType = NewEquipType;
	CurrentGun = EquipGuns[NewEquipType];

	if (OnChangeEquip.IsBound())
	{
		OnChangeEquip.Execute(CurrentEquipType, CurrentGun);
	}
	
	for (const TPair<EEquipType, APTGun*>& Pair : EquipGuns)
	{
		Pair.Value->SetActorHiddenInGame(Pair.Key != CurrentEquipType);
	}
}

void UPTEquipmentComponent::SetEquipment(FString GunDataKey, EEquipType EquipType)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	APTGun* Gun = GetWorld()->SpawnActor<APTGun>(EquipType == EEquipType::Main ?  MainGunClass : SubGunClass);
	Gun->SetOwner(OwnerCharacter);
	Gun->Init(GunDataKey);
	
	if (EquipGuns.Contains(EquipType))
	{
		EquipGuns.Remove(EquipType);
	}
	EquipGuns.Add(EquipType, Gun);

	Gun->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
}

