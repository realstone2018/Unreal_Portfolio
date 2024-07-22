#include "PTGameModeBase.h"
#include "PTActor/PTProjectile.h"
#include "Engine/World.h"
#include "Manager/PTSpawnManager.h"
#include "UObject/UObjectGlobals.h"
#include "Manager/PTObjectPoolManager.h"
#include "Character/PTMonster.h"
#include "GameData/ObjectPoolData.h"
#include "Character/PTPlayerController.h"
#include "PTActor/PTStructure.h"

APTGameModeBase::APTGameModeBase()
{
	PoolManager = CreateDefaultSubobject<UPTObjectPoolManager>(TEXT("ObjectPoolManager"));
	SpawnManager = CreateDefaultSubobject<UPTSpawnManager>(TEXT("SpawnManager"));

	static ConstructorHelpers::FClassFinder<APTStructure> MainStationClassRef(TEXT("/Game/Project2/Blueprint/Structure/BP_MainStation.BP_MainStation_C"));
	if (MainStationClassRef.Class)
	{
		MainStationClass = MainStationClassRef.Class;
	}	
}

void APTGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	MainStation = GetWorld()->SpawnActor<APTStructure>(MainStationClass, FVector(0.f, 0.f, -350.f), FRotator::ZeroRotator);
	check(MainStation);
	
	PoolManager->Init(GetWorld());
	PoolManager->SetUpPool<APTMonster>(EPoolType::Monster);
	PoolManager->SetUpPool<APTProjectile>(EPoolType::Projectile);

	SpawnManager->Init(PoolManager);
	
	TimerStart();
}


void APTGameModeBase::TimerStart()
{
	FTimerHandle StageTimerHandle;
	FTimerDelegate StageEndDelegate = FTimerDelegate::CreateUObject(this, &APTGameModeBase::TimerEnd);

	//TODO: Stage 데이터 받아서 돌리기 
	GetWorldTimerManager().SetTimer(StageTimerHandle, StageEndDelegate, 3.f, false);
}

void APTGameModeBase::TimerEnd()
{
	//SpawnManagerV3->SpawnObject<APTMonster>(FRotator::ZeroRotator, FVector(4500.f, 0.f, -200.f));
	SpawnManager->SpawnMonsterWave(FVector(4500.f, 0.f, 50.f), 1);
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
