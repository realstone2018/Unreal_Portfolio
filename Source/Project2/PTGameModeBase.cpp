#include "PTGameModeBase.h"
#include "PTActor/PTProjectile.h"
#include "Engine/World.h"
#include "Manager/PTSpawnManager.h"
#include "Manager/PTObjectPoolManager.h"
#include "Character/PTMonster.h"
#include "GameData/ObjectPoolData.h"
#include "Character/PTPlayerController.h"
#include "Kismet/GameplayStatics.h"
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

	SpawnManager->Init(GetWorld(), PoolManager);

}

void APTGameModeBase::StartPlay()
{
	Super::StartPlay();

	GameStart();	
}

void APTGameModeBase::GameStart()
{
	TimerStart();
	
}

void APTGameModeBase::GameClear()
{
	UE_LOG(LogTemp, Display, TEXT("APTGameModeBase::GameClear()"));

	APTPlayerController* PlayerController = Cast<APTPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->GameClear();

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}	
}

void APTGameModeBase::GameOver()
{
	UE_LOG(LogTemp, Display, TEXT("APTGameModeBase::GameOver()"));

	APTPlayerController* PlayerController = Cast<APTPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->GameOver();

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void APTGameModeBase::OnPlayerDead()
{
	//TODO: N초 후 플레이어 부활 
	
}

void APTGameModeBase::TimerStart()
{
	FTimerDelegate StageEndDelegate = FTimerDelegate::CreateUObject(this, &APTGameModeBase::TimerEnd);

	GetWorldTimerManager().SetTimer(StageTimerHandle, StageEndDelegate, StageClearTime, false);

	UE_LOG(LogTemp, Display, TEXT("APTGameModeBase::TimerStart() - %f"), GetWorldTimerManager().GetTimerRemaining(StageTimerHandle));

	APTPlayerController* PlayerController = Cast<APTPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->TimerStart();
	}
}

void APTGameModeBase::TimerEnd()
{
	//SpawnManagerV3->SpawnObject<APTMonster>(FRotator::ZeroRotator, FVector(4500.f, 0.f, -200.f));
	//SpawnManager->SpawnMonsterWave(MainStation->GetActorLocation(), 10);

	UE_LOG(LogTemp, Display, TEXT("APTGameModeBase::TimerEnd()"));
	
	GameClear();		
}

float APTGameModeBase::GetRemainTime()
{
	if (!StageTimerHandle.IsValid())
	{
		return 0.f; 
	}
	
	return FMath::Max(0.f, GetWorldTimerManager().GetTimerRemaining(StageTimerHandle));
}
