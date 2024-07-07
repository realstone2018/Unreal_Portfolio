#include "Manager/MonsterSpawnManager.h"
#include "Character/PTScorch.h"
#include "Character/PTMonster.h"

UMonsterSpawnManager::UMonsterSpawnManager()
{
	// static ConstructorHelpers::FClassFinder<APTMonster> ScorchClassRef(TEXT("/Game/Project2/Character/BP_Monster.BP_Monster_C"));
	// if (ScorchClassRef.Class) {
	// 	MonsterClass = ScorchClassRef.Class;
	// 	
	// 	UE_LOG(LogTemp, Log, TEXT("Monster class found: %s"), *MonsterClass->GetName());
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("MonsterSpawnManager::UMonsterSpawnManager() - Fail Class Find!"));
	// }
	
}

// void UMonsterSpawnManager::SpawnMonster(int num)
// {
// 	const FTransform SpawnTransform(FVector::ZeroVector + FVector::UpVector * 88.0f);
//
// 	// MonsterClass를 사용하여 APTMonster 블루프린트 인스턴스를 스폰
// 	APTMonster* SpawnedMonster = GetWorld()->SpawnActorDeferred<APTMonster>(MonsterClass, SpawnTransform);
// 	if (SpawnedMonster)
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("Monster spawned successfully."));
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Failed to spawn monster."));
// 	}
// }
