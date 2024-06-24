#include "PTGameModeBase.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Manager/MonsterSpawnManager.h"
#include "UObject/UObjectGlobals.h"
#include "Manager/PTObjectPoolManager.h"
#include "Character/PTMonster.h"	

APTGameModeBase::APTGameModeBase()
{
	UE_LOG(LogTemp, Display, TEXT("APTGameModeBase::APTGameModeBase"));

	
	//TODO: Wave타이머 가동 

	MonsterSpawnManager = NewObject<UMonsterSpawnManager>();

	static ConstructorHelpers::FClassFinder<APTMonster> ScorchClassRef(TEXT("/Game/Project2/Character/BP_Monster.BP_Monster_C"));
	if (ScorchClassRef.Class) {
		MonsterClass = ScorchClassRef.Class;
		
		UE_LOG(LogTemp, Log, TEXT("Monster class found: %s"), *MonsterClass->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("MonsterSpawnManager::UMonsterSpawnManager() - Fail Class Find!"));
	}

	PoolManager = CreateDefaultSubobject<UPTObjectPoolManager>(TEXT("ObjectPoolManager"));

	// // 예제: 몬스터 풀에서 객체 가져오기
	// AActor* Monster = PoolManager->GetPooledObject();
	// if (Monster)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("Monster spawned from pool."));
	// }
}

void APTGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//TODO: 풀링 DataAsset을 받아서 로드 
	PoolManager->Init(GetWorld());
	PoolManager->InitializePool<APTMonster>(EPoolListType::Monster, MonsterClass, 3);

	TimerStart();
}

void APTGameModeBase::PawnDead(APawn* Victim)
{
	if (LoseCondition(Victim))
	{
		StageLose();
		return;
	}

	//TODO: Wave클리어, 다음 Wave 카운터 시작
	
}


void APTGameModeBase::TimerStart()
{
	FTimerHandle StageTimerHandle;
	FTimerDelegate StageEndDelegate = FTimerDelegate::CreateUObject(this, &APTGameModeBase::TimerEnd);

	//TODO: Stage 데이터 받아서 돌리기 
	GetWorldTimerManager().SetTimer(StageTimerHandle, StageEndDelegate, 3.f, true);
}

void APTGameModeBase::TimerEnd()
{
	UE_LOG(LogTemp, Display, TEXT("PTGameModeBase::TimerEnd"));
	
	SpawnMonster(1);
}

bool APTGameModeBase::WinCondition()
{
	//TODO: 클리어 타이머가 종료된 경우
	return false;
}

bool APTGameModeBase::LoseCondition(APawn* Victim)
{
	//TODO: 기지가 죽은 경우 
	return false;
}

void APTGameModeBase::StageWin()
{
	//TODO: Stage Clear UI 출력 
	//Controller->GameHasEnded(Controller->GetPawn(), bIsWin = true);
}

void APTGameModeBase::StageLose()
{
	//TODO: Stage Lose UI 출력
	//Controller->GameHasEnded(Controller->GetPawn(), bIsWin = false);
}


void APTGameModeBase::EndGame()
{
	//TODO: StageWin, Lose 결과창에서 확인 눌른 경우 호출되서 아웃게임으로 이동

	
}

void APTGameModeBase::SpawnMonster(int num)
{
	const FTransform SpawnTransform(FVector(4500.f, 0.f, -200.f));

	APTMonster* SpawnedMonster = PoolManager->GetPooledObject<APTMonster>(EPoolListType::Monster, SpawnTransform);
	if (SpawnedMonster)
	{
		SpawnedMonster->OnDead.BindLambda([this](AActor* DeadActor)
		{
			PoolManager->ReturnPooledObject(DeadActor);	
		});
		
		UE_LOG(LogTemp, Log, TEXT("Monster spawned successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn monster."));
	}
}
