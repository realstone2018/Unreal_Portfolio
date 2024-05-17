#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);
	
	if (LoseCondition(PawnKilled))
	{
		EndGame(false);
		return;
	}

	if (WinCondition())
	{
		EndGame(true);	
	}
}

bool AKillEmAllGameMode::WinCondition()
{
	// 몬스터가 많아질수록 성능이 안좋아진다.
	//TODO: 몬스터 스폰 컨트롤러를 만든후, 스폰한 몬스터들을 TArray등으로 캐싱해서 관리 및 카운트해서 컨디션 체ㅐ크하기
	for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
	{
		if (!Controller->IsDead())
		{
			return false;
		}
	}

	return true;
}

bool AKillEmAllGameMode::LoseCondition(APawn* PawnKilled)
{
	// 플레이어가 죽은 경우 게임 종료
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	return PlayerController != nullptr;
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	// TActorRange<>(): 월드의 모든 지정 클래스 인스턴스를 가져온다.
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = bIsPlayerWinner == Controller->IsPlayerController();
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
