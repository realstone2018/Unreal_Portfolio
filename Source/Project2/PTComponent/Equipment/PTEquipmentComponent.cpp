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
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	APTGun* MainGun = GetWorld()->SpawnActor<APTGun>(MainGunClass);
	EquipGuns.Add(EEquipType::Main, MainGun);
	MainGun->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	MainGun->SetOwner(OwnerCharacter);
	MainGun->SetGunData(UPTGameDataSingleton::Get().GetGunData("BaseRifle"));
	
	APTGun* SubGun = GetWorld()->SpawnActor<APTGun>(SubGunClass);
	EquipGuns.Add(EEquipType::Sub, SubGun);
	SubGun->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	SubGun->SetOwner(OwnerCharacter);
	SubGun->SetGunData(UPTGameDataSingleton::Get().GetGunData("BaseLauncher"));

	//Gun->SetGunData(UPTGameDataSingleton::Get().GetGunData("BaseRifle"));

	ChangeEquipment(EEquipType::Main);
}

void UPTEquipmentComponent::ChangeEquipment(EEquipType NewEquipType)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEquipType"), true);
	//UE_LOG(LogTemp, Display, TEXT("UPTEquipmentComponent::ChangeEquipment - CurrentEquipType: %s / NewEquipType: %s"), *EnumPtr->GetNameStringByValue((uint8)CurrentEquipType),*EnumPtr->GetNameStringByValue((uint8)NewEquipType));
	
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

