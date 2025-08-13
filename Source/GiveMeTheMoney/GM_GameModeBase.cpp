
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
	// �⺻�� ����
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

	// "AI" �±׸� ���� ��� ���͸� ã�Ƽ� �迭�� ����
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("AI"), SpawnVolumes);

	// GameState �ʱ�ȭ
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (MyGS)
	{
		MyGS->MaxWaveIndex = MaxWaveIndex;
		MyGS->MinKillCount = MinKillCount;

		// GameState�� ���� ��� Ŭ���̾�Ʈ���� ���� ���� ��ȣ�� �����ϴ�.
		if (MyGS->GetClass()->ImplementsInterface(UGM_Interface::StaticClass()))
		{
			IGM_Interface::Execute_OnGameStart(MyGS);
		}
	}

	// ù ���̺� ����
	StartWave();

	// HUD ������Ʈ Ÿ�̸� ���� (1�ʸ���)
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

	// ���̺� �ð� Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AGM_GameModeBase::OnWaveTimeUp, MyGS->WaveDuration, false);
}

void AGM_GameModeBase::EndWave(bool bIsCompleted)
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (!MyGS) return;

	// ���̺� ����/���� ���� Ȯ��
	if (bIsCompleted && MyGS->CurrentKillCount >= MyGS->MinKillCount)
	{
		// ������ ���̺꿴���� Ȯ��
		if (MyGS->CurrentWaveIndex >= MyGS->MaxWaveIndex)
		{
			OnGameOver(true); // ���� �¸�
		}
		else
		{
			StartWave(); // ���� ���̺� ����
		}
	}
	else
	{
		OnGameOver(false); // ���� ����
	}
}

void AGM_GameModeBase::OnWaveTimeUp()
{
	EndWave(false); // �ð��� �� �Ǹ� ���̺� ����
}

void AGM_GameModeBase::SpawnEnemies()
{
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (!MyGS || SpawnVolumes.Num() == 0 || !EnemyClass) return;

	for (int32 i = 0; i < MyGS->SpawnedMonster; ++i)
	{
		// ������ ���� ���� ����
		const int32 RandomVolumeIndex = FMath::RandRange(0, SpawnVolumes.Num() - 1);
		AActor* TargetVolume = SpawnVolumes[RandomVolumeIndex];

		// �������� BoxComponent ã��
		UBoxComponent* BoxComponent = TargetVolume->FindComponentByClass<UBoxComponent>();
		if (BoxComponent)
		{
			// BoxComponent�� ��� ������ ������ ��ġ ����
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
	// ���� ���� ���� (����� �������)
}

void AGM_GameModeBase::OnGameOver(bool bIsVictory)
{
	// ��� Ÿ�̸� ����
	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (bIsVictory)
	{
		UE_LOG(LogTemp, Log, TEXT("Game Won!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Game Over!"));
	}

	// GameState�� ���� ��� Ŭ���̾�Ʈ���� ���� ���� ��ȣ�� �����ϴ�.
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (MyGS && MyGS->GetClass()->ImplementsInterface(UGM_Interface::StaticClass()))
	{
		IGM_Interface::Execute_OnGameOver(MyGS, bIsVictory);
	}

	//���� ���� ���� ���� �߰�
}

void AGM_GameModeBase::UpdateHUD()
{
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (MyGS && MyGS->GetClass()->ImplementsInterface(UGM_Interface::StaticClass()))
	{
		// GameState�� �ֽ� ������ �������̽� �Լ� ȣ��
		IGM_Interface::Execute_UpdateCoin(MyGS, MyGS->TotalCoin);
		IGM_Interface::Execute_UpdateWave(MyGS, MyGS->CurrentWaveIndex, MyGS->MaxWaveIndex);
		IGM_Interface::Execute_UpdateGameTimer(MyGS, GetWorld()->GetTimeSeconds()); // ��ü ���� �ð�
		IGM_Interface::Execute_UpdateWaveTimer(MyGS, GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle));
		IGM_Interface::Execute_UpdateKillCount(MyGS, MyGS->CurrentKillCount, MyGS->MinKillCount);
	}
}
