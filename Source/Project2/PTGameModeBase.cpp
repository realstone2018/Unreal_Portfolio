#include "PTGameModeBase.h"
#include "PTActor/PTProjectile.h"
#include "Engine/World.h"
#include "Manager/MonsterSpawnManager.h"
#include "UObject/UObjectGlobals.h"
#include "Manager/PTObjectPoolManager.h"
#include "Character/PTMonster.h"
#include "GameData/ObjectPoolData.h"
#include "Character/PTPlayerController.h"

APTGameModeBase::APTGameModeBase()
{
	MonsterSpawnManager = NewObject<UMonsterSpawnManager>();
	PoolManager = CreateDefaultSubobject<UPTObjectPoolManager>(TEXT("ObjectPoolManager"));
}

void APTGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	PoolManager->Init(GetWorld());
	
	TimerStart();
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
	SpawnMonster();
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

void APTGameModeBase::SpawnMonster()
{
	const FTransform SpawnTransform(FVector(4500.f, 0.f, -200.f));

	// ReturnPool부분 
	APTMonster* SpawnedMonster = PoolManager->GetPooledObject<APTMonster>(EPoolType::Monster, SpawnTransform);
	ensure(SpawnedMonster);

	SpawnedMonster->OnDead.BindUObject(this, &APTGameModeBase::ReturnPoolMonster);
}

void APTGameModeBase::ReturnPoolMonster(AActor* Monster)
{
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle,
		FTimerDelegate::CreateLambda([this, Monster](){
			PoolManager->ReturnPooledObject(EPoolType::Monster, Monster);
		}
	), 5.0f, false);
}

APTProjectile* APTGameModeBase::SpawnProjectile(int ProjectileCode, FVector SpawnPosition, FRotator SpawnRotator)
{
	const FTransform SpawnTransform(SpawnRotator, SpawnPosition);

	APTProjectile* SpawnedProjectile = PoolManager->GetPooledObject<APTProjectile>(EPoolType::Projectile, SpawnTransform);
	ensure(SpawnedProjectile);

	SpawnedProjectile->OnDead.BindLambda([&](AActor* Projectile)
	{
		PoolManager->ReturnPooledObject(EPoolType::Projectile, Projectile);
	});

	return SpawnedProjectile;
}
