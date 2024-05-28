#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PTCharacterStat.h"
#include "PTGunData.h"
#include "PTGameDataSingleton.generated.h"

//싱글톤 Get함수가 실패한 경우
DECLARE_LOG_CATEGORY_EXTERN(LogPTGameDataSingleton, Error, All);

UCLASS()
class PROJECT2_API UPTGameDataSingleton : public UObject
{
	GENERATED_BODY()

public:
	UPTGameDataSingleton();

	static UPTGameDataSingleton& Get();
	
	FORCEINLINE FPTCharacterStat GetCharacterStat(int32 InLevel) const {
		return CharacterStatArray.IsValidIndex(InLevel - 1)? CharacterStatArray[InLevel - 1] : FPTCharacterStat();}
	
	FORCEINLINE FPTGunData GetGunData(FName GunName) const {
		return GunDataMap.Contains(GunName)? GunDataMap[GunName] : FPTGunData();}
	
	UPROPERTY()
	int32 CharacterMaxLevel;
	
private:
	void LoadCharacterStatData();
	void LoadGunData();
	
	TArray<FPTCharacterStat> CharacterStatArray;
	TMap<FName, FPTGunData> GunDataMap;
};
