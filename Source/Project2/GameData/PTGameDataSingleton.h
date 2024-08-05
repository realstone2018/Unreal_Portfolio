#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PTCharacterStat.h"
#include "PTGunData.h"
#include "PTMonsterStat.h"
#include "PTProjectileData.h"
#include "PTGameDataSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPTGameDataSingleton, Error, All);

UCLASS()
class PROJECT2_API UPTGameDataSingleton : public UObject
{
	GENERATED_BODY()

public:
	UPTGameDataSingleton();

	static UPTGameDataSingleton& Get();
	
	FPTCharacterStat GetCharacterStat(int32 InLevel) const;
	
	FPTMonsterStat GetMonsterStat(FName MonsterDataKey) const;
	
	FPTGunData GetGunData(FName GunName) const;

	FPTProjectileData GetProjectileData(FName ProjectileName) const;
	
	UPROPERTY()
	int32 CharacterMaxLevel;
	
private:
	void LoadCharacterStatData();
	void LoadMonsterData();
	void LoadGunData();
	void LoadProjectileData();

	UPROPERTY()
	TArray<FPTCharacterStat> CharacterStatArray;
	
	UPROPERTY()
	TMap<FName, FPTMonsterStat> MonsterStatMap;

	UPROPERTY()
	TMap<FName, FPTGunData> GunDataMap;

	UPROPERTY()
	TMap<FName, FPTProjectileData> ProjectileDataMap;
};
