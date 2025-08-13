
#include "GM_GameModeBase.h"
#include "GM_GameStateBase.h"
#include "GM_Interface.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

AGM_GameModeBase::AGM_GameModeBase()
{
	// 기본값 설정
	EnemyClass = nullptr;
	WaveDuration = 30.0f;
	InitialEnemyCount = 10;
	EnemyCountIncrease = 5;
	MaxWaveIndex = 5;
	MinKillCount = 10;
}

void AGM_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// "AI" 태그를 가진 모든 액터를 찾아서 배열에 저장
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("AI"), SpawnVolumes);

	// GameState 초기화
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (MyGS)
	{
		MyGS->MaxWaveIndex = MaxWaveIndex;
		MyGS->MinKillCount = MinKillCount;

		// GameState를 통해 모든 클라이언트에게 게임 시작 신호를 보냅니다.
		if (MyGS->GetClass()->ImplementsInterface(UGM_Interface::StaticClass()))
		{
			IGM_Interface::Execute_OnGameStart(MyGS);
		}
	}

	// 첫 웨이브 시작
	StartWave();

	// HUD 업데이트 타이머 시작 (1초마다)
	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &AGM_GameModeBase::UpdateHUD, 1.0f, true);
}

void AGM_GameModeBase::StartWave()
{
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (!MyGS) return;

	MyGS->CurrentWaveIndex++;
	MyGS->WaveDuration = WaveDuration;
	MyGS->CurrentKillCount = 0;
	MyGS->SpawnedMonster = InitialEnemyCount + (EnemyCountIncrease * (MyGS->CurrentWaveIndex - 1));

	SpawnEnemies();

	// 웨이브 시간 타이머 시작
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AGM_GameModeBase::OnWaveTimeUp, MyGS->WaveDuration, false);
}

void AGM_GameModeBase::EndWave(bool bIsCompleted)
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (!MyGS) return;

	// 웨이브 성공/실패 조건 확인
	if (bIsCompleted && MyGS->CurrentKillCount >= MyGS->MinKillCount)
	{
		// 마지막 웨이브였는지 확인
		if (MyGS->CurrentWaveIndex >= MyGS->MaxWaveIndex)
		{
			OnGameOver(true); // 게임 승리
		}
		else
		{
			StartWave(); // 다음 웨이브 시작
		}
	}
	else
	{
		OnGameOver(false); // 게임 오버
	}
}

void AGM_GameModeBase::OnWaveTimeUp()
{
	EndWave(false); // 시간이 다 되면 웨이브 실패
}

void AGM_GameModeBase::SpawnEnemies()
{
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (!MyGS || SpawnVolumes.Num() == 0 || !EnemyClass) return;

	for (int32 i = 0; i < MyGS->SpawnedMonster; ++i)
	{
		// 랜덤한 스폰 볼륨 선택
		const int32 RandomVolumeIndex = FMath::RandRange(0, SpawnVolumes.Num() - 1);
		AActor* TargetVolume = SpawnVolumes[RandomVolumeIndex];

		// 볼륨에서 BoxComponent 찾기
		UBoxComponent* BoxComponent = TargetVolume->FindComponentByClass<UBoxComponent>();
		if (BoxComponent)
		{
			// BoxComponent의 경계 내에서 랜덤한 위치 생성
			FVector SpawnOrigin = BoxComponent->Bounds.Origin;
			FVector SpawnExtent = BoxComponent->Bounds.BoxExtent;
			FVector SpawnLocation = FMath::RandPointInBox(FBox(SpawnOrigin - SpawnExtent, SpawnOrigin + SpawnExtent));

			GetWorld()->SpawnActor<ACharacter>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
		}
	}
}

void AGM_GameModeBase::OnEnemyKilled()
{
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (MyGS)
	{
		MyGS->CurrentKillCount++;
	}
}

void AGM_GameModeBase::AddCoin(int32 Amount)
{
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (MyGS)
	{
		MyGS->TotalCoin += Amount;
	}
}

void AGM_GameModeBase::OnCoinCollected()
{
	// 코인 수집 로직 (현재는 비어있음)
}

void AGM_GameModeBase::OnGameOver(bool bIsVictory)
{
	// 모든 타이머 중지
	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (bIsVictory)
	{
		UE_LOG(LogTemp, Log, TEXT("Game Won!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Game Over!"));
	}

	// GameState를 통해 모든 클라이언트에게 게임 오버 신호를 보냅니다.
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (MyGS && MyGS->GetClass()->ImplementsInterface(UGM_Interface::StaticClass()))
	{
		IGM_Interface::Execute_OnGameOver(MyGS, bIsVictory);
	}

	//게임 오버 관련 로직 추가
}

void AGM_GameModeBase::UpdateHUD()
{
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (MyGS && MyGS->GetClass()->ImplementsInterface(UGM_Interface::StaticClass()))
	{
		// GameState의 최신 정보로 인터페이스 함수 호출
		IGM_Interface::Execute_UpdateCoin(MyGS, MyGS->TotalCoin);
		IGM_Interface::Execute_UpdateWave(MyGS, MyGS->CurrentWaveIndex, MyGS->MaxWaveIndex);
		IGM_Interface::Execute_UpdateGameTimer(MyGS, GetWorld()->GetTimeSeconds()); // 전체 게임 시간
		IGM_Interface::Execute_UpdateWaveTimer(MyGS, GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle));
		IGM_Interface::Execute_UpdateKillCount(MyGS, MyGS->CurrentKillCount, MyGS->MinKillCount);
	}
}
