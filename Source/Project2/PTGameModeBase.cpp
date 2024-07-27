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

	MainStation = GetWorld()->SpawnActor<APTStructure>(MainStationClass, FVector::ZeroVector, FRotator::ZeroRotator);
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
	StageTimer();
	MonsterWaveTimer();
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

void APTGameModeBase::OnPlayerDead(ACharacter* ControllCharacter)
{
	// 더이상 어떤 행동도 하지 않도록 컨트롤러를 폰에서 분리 
	ControllCharacter->DetachFromControllerPendingDestroy();
	
	PlayerRespawnTimer();
}

void APTGameModeBase::PlayerRespawnTimer()
{
	//TODO: N초 후 플레이어 부활 
	//TODO: UIWidget 화면 전체 회색 필터 적용?  + 부활 카운트 다운  
	
	GetWorldTimerManager().SetTimer(PlayerRespawnTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		RestartPlayer(GetWorld()->GetFirstPlayerController());
	}), PlayerRespawnDelay, false);

	APTPlayerController* PlayerController = Cast<APTPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->PlayerRespawn();
	}	
}


void APTGameModeBase::RestartPlayer(AController* NewPlayer)
{
	UE_LOG(LogGameMode, Warning, TEXT("APTGameModeBase::RestartPlayer()"));
	
	Super::RestartPlayer(NewPlayer);


	//TODO: UI 갱신

	
}

void APTGameModeBase::StageTimer()
{
	FTimerDelegate StageEndDelegate = FTimerDelegate::CreateUObject(this, &APTGameModeBase::GameClear);
	GetWorldTimerManager().SetTimer(StageTimerHandle, StageEndDelegate, StageClearTime, false);
	
	APTPlayerController* PlayerController = Cast<APTPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->TimerStart();
	}
}

float APTGameModeBase::GetStageRemainTime()
{
	if (!StageTimerHandle.IsValid())
	{
		return 0.f; 
	}
	
	return FMath::Max(0.f, GetWorldTimerManager().GetTimerRemaining(StageTimerHandle));
}

float APTGameModeBase::GetPlayerRespawnRemainTime()
{
	if (!PlayerRespawnTimerHandle.IsValid())
	{
		return 0.f;
	}

	return FMath::Max(0.f, GetWorldTimerManager().GetTimerRemaining(PlayerRespawnTimerHandle));
}

void APTGameModeBase::MonsterWaveTimer()
{
	FTimerHandle MonsterWaveTimerHandle;

	SpawnManager->SpawnMonsterWave(MainStation->GetActorLocation(), 1);
	
	SpawnManager->SpawnMonsterWave(MainStation->GetActorLocation(), 3);
	SpawnManager->SpawnMonsterWave(MainStation->GetActorLocation(), 3);
	
	
	 GetWorldTimerManager().SetTimer(MonsterWaveTimerHandle, FTimerDelegate::CreateLambda([this]()
	 {
	 	SpawnManager->SpawnMonsterWave(MainStation->GetActorLocation(), 3);
	 	SpawnManager->SpawnMonsterWave(MainStation->GetActorLocation(), 2);
	 	SpawnManager->SpawnMonsterWave(MainStation->GetActorLocation(), 4);
	 }), 18.f, true);
}
