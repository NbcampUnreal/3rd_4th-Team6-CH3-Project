// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/WaveGameMode.h"

#include "Blueprint/UserWidget.h"
#include "GM_Characters/ShooterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GM_Monster/Monster.h"
#include "Spawners/SpawnVolume.h"
#include "UI/WaveHUDWidget.h"

AGM_GameModeBase::AGM_GameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
	// 소환될 몬스터, 리스폰 간격(시간), 웨이브 타임
	// Waves = {
	// 	{ 5, 1.0f, 60},   // Wave 1
	// 	{ 8, 0.9f, 50},  // Wave 2
	// 	{ 12, 0.8f, 40},  // Wave 3
	// 	{ 16, 0.75f, 40}, // Wave 4
	// 	{ 22, 0.7f, 40}   // Wave 5
	// };
}

void AGM_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// 스폰 볼륨 수집
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), Found);
	for (AActor* A : Found)
	{
		if (auto* V = Cast<ASpawnVolume>(A))
		{
			SpawnVolumes.Add(V);
		}
	}

	// HUD 생성
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UWaveHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			HUDWidget->ShowStartScreen();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("SpawnVolumes collected: %d"), SpawnVolumes.Num());

	bGameActive = false;
	CurrentWaveIndex = -1;
	KillCount = 0;
	Score = 0;

	// INPUT MODE 설정 - UI 조작 가능
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(HUDWidget->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}
}

void AGM_GameModeBase::StartNextWave()
{
	// 게임 level 안에
	// waves = 3개라고 정의 -> bp -> 첫 번째 웨이브 시작.
	++CurrentWaveIndex;
	if (!Waves.IsValidIndex(CurrentWaveIndex)) // 잘못된 인덱스 -> ! -> 실행 // 0,1,2 -> 3
	{
		GameOver(true); // 모든 웨이브 클리어했다 -> 승리
		return;
	}

	const FWaveConfig& Wave = Waves[CurrentWaveIndex]; // Wave - 몬스터 소환 개수, 스폰 시간, 웨이브 타임
	RemainingToSpawn = Wave.EnemyCount;
	AliveEnemies = 0; //  초기화

	RemainingTimeSeconds = Wave.TimeLimitSeconds;
	UE_LOG(LogTemp, Warning, TEXT("[GM] Wave %d Start -> Time = %d s"),
		CurrentWaveIndex + 1, RemainingTimeSeconds);

	if (HUDWidget)
	{
		HUDWidget->ShowWaveStart(CurrentWaveIndex + 1);
		HUDWidget->UpdateTime(RemainingTimeSeconds);
		HUDWidget->UpdateStats(KillCount, Score);
	}

	if (Wave.EnemyCount > 0 && MonsterClass)
	{
		GetWorldTimerManager().SetTimer(
			WaveSpawnTimerHandle,
			this,
			&AGM_GameModeBase::SpawnOneEnemy,
			Wave.SpawnInterval,
			true
		);
	}
}

void AGM_GameModeBase::SpawnOneEnemy()
{
	if (!bGameActive) return;

	if (RemainingToSpawn <= 0)
	{
		GetWorldTimerManager().ClearTimer(WaveSpawnTimerHandle);
		return;
	}

	FVector SpawnLoc;
	if (!GetRandomSpawnLocation(SpawnLoc))
	{
		UE_LOG(LogTemp, Warning, TEXT("No spawn volume available"));
		return;
	}

	FRotator Rot = FRotator::ZeroRotator;
	if (AMonster* Spawned = GetWorld()->SpawnActor<AMonster>(MonsterClass, SpawnLoc, Rot))
	{
		++AliveEnemies; // 1 -> 2
		--RemainingToSpawn; // 10 -> 9 -> 8
		// 몬스터가 죽을 때 GameMode->ReportEnemyDeath() 호출하도록 MonsterAI 구현해야지 정상 작동!
	}
}

void AGM_GameModeBase::CheckWaveEnd()
{
	if (!bGameActive) return;

	if (AliveEnemies <= 0 && RemainingToSpawn <= 0)
	{
		StartNextWave();
	}
}

void AGM_GameModeBase::TickGameTimer()
{
	if (!bGameActive) return;

	// 남은 시간 감소
	--RemainingTimeSeconds;

	if (HUDWidget)
	{
		HUDWidget->UpdateTime(RemainingTimeSeconds);
	}

	// 디버그 출력
	UE_LOG(LogTemp, Warning, TEXT("Remaining Time: %d"), RemainingTimeSeconds);

	if (RemainingTimeSeconds <= 0) // 60 -> 59 -> 58 -> 0, -1
	{
		RemainingTimeSeconds = 0;
		GameOver(false); // 시간초과 - 패배
	}
}

void AGM_GameModeBase::GameOver(bool bVictory)
{
	if (!bGameActive) return;
	bGameActive = false;

	// 중복 호출 방지
	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveSpawnTimerHandle);

	// 더 이상 웨이브 진행되지 않도록 남은 수치 정리
	RemainingToSpawn = 0;
	AliveEnemies = 0;

	if (HUDWidget)
	{
		HUDWidget->ShowGameOver(bVictory, Score, KillCount);
	}

	// 플레이어 멈추기 & 크로스헤어 숨김
	if (AShooterPlayer* Player = Cast<AShooterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Player->StopMovement();
		Player->ShowCrosshair(false);
	}

	// 디버그 출력
	if (bVictory)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over: VICTORY! Final Score: %d Kills: %d"), Score, KillCount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over: DEFEAT (Time Out). Final Score: %d Kills: %d"), Score, KillCount);
	}

	// INPUT MODE 다시 UI로 변경
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}
}

bool AGM_GameModeBase::GetRandomSpawnLocation(FVector& OutLocation) const
{
	if (SpawnVolumes.Num() == 0)
		return false;

	int32 Index = FMath::RandRange(0, SpawnVolumes.Num() - 1);
	if (ASpawnVolume* Volume = SpawnVolumes[Index])
	{
		OutLocation = Volume->GetRandomPointInVolume();
		return true;
	}
	return false;
}

void AGM_GameModeBase::StartGame()
{
	if (bGameActive) return;
	bGameActive = true;

	// 크로스헤어 보이기
	if (AShooterPlayer* Player = Cast<AShooterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Player->ShowCrosshair(true);
	}

	// INPUT MODE 변경 - 게임 플레이 모드
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
	}

	// 시작, 재시작시 초기화
	CurrentWaveIndex = -1;
	KillCount = 0;
	Score = 0;

	GetWorldTimerManager().ClearTimer(GameTimerHandle); // 한번 기존 타이머 클리어
	GetWorldTimerManager().SetTimer(GameTimerHandle, this,
		&AWaveGameMode::TickGameTimer, 1.f, true);

	// 첫 웨이브 시작 직후 HUD 동기화 (WaveStart 안에서 이미 일부 함)
	if (HUDWidget)
	{
		HUDWidget->UpdateStats(KillCount, Score);
		HUDWidget->UpdateTime(RemainingTimeSeconds);
	}

	StartNextWave();
}

void AGM_GameModeBase::ReportEnemyDeath()
{
	if (!bGameActive) return;

	--AliveEnemies;
	++KillCount;
	Score += 100;

	if (HUDWidget)
	{
		HUDWidget->UpdateStats(KillCount, Score);
	}

	CheckWaveEnd();
}
