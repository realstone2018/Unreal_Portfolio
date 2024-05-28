#include "GameData/PTGameDataSingleton.h"

//싱글톤 Get함수가 실패한 경우, 구현
DEFINE_LOG_CATEGORY(LogPTGameDataSingleton);

UPTGameDataSingleton::UPTGameDataSingleton()
{
	LoadCharacterStatData();
	LoadGunData();
}

void UPTGameDataSingleton::LoadCharacterStatData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/Project2/GameData/PTCharacterStatTable.PTCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		//레벨 데이터로, Key가 없어도 인덱스로 각 레벨에 접근가능 -> Key를 버리고 Value만 저장
		TArray<uint8*> ValueArray;
		//Value만 배열로 가져와, 알고리즘의 Transform라이브러리를 사용해 저장
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatArray,
			[](uint8* Value)
			{
				return *reinterpret_cast<FPTCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatArray.Num();
	ensure(CharacterMaxLevel > 0);
}

void UPTGameDataSingleton::LoadGunData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/Project2/GameData/PTGunDataTable.PTGunDataTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable= DataTableRef.Object;
		for (const auto& Row : DataTable->GetRowMap())
		{
			FName RowName = Row.Key;
			FPTGunData* GunData = reinterpret_cast<FPTGunData*>(Row.Value);	// uint8*를 FPTGunData*로 변환

			if (GunData)
			{
				GunDataMap.Add(RowName, *GunData);
			}
		}

		check(GunDataMap.Num() > 0);
	}
}

UPTGameDataSingleton& UPTGameDataSingleton::Get()
{
	//CastChecked로 형변환이 되는지 (가져온게 유요한 객체인지) 강력하게 확인
	UPTGameDataSingleton* Singleton = CastChecked<UPTGameDataSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	//싱글톤 Get함수가 실패한 경우
	UE_LOG(LogPTGameDataSingleton, Error, TEXT("Invalid Game Singleton"));
	//함수 완성을 위한 return, 실제 사용되지는 않음 
	return *NewObject<UPTGameDataSingleton>();
}


