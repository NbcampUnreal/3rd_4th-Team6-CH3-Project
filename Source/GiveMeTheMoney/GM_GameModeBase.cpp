
#include "GM_GameModeBase.h"
#include "GM_GameStateBase.h"
#include "GM_Character.h"
#include "GM_Interface.h"
#include "GM_SpawnVolume.h"
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
	//UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("AI"), SpawnVolumes);

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

	MyGS->WaveDuration = WaveDuration;
	MyGS->CurrentKillCount = 0;
	
	//MyGS->SpawnedMonster = InitialEnemyCount + (EnemyCountIncrease * (MyGS->CurrentWaveIndex - 1));
	// 
	//SpawnVolume �����ϱ�
	SpawnEnemies();

	MyGS->CurrentWaveIndex++;	// ���̺� ����

	// ���̺� �ð� Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AGM_GameModeBase::OnWaveTimeUp, MyGS->WaveDuration, false);
	
	//SpawnEnemies();
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
			UE_LOG(LogTemp, Error, TEXT("OnGameOver(ture)"))
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
	

	if (!MyGS || EnemyClass == nullptr) return;	// GameState ���� EnemyClass ������ ���� X

	MyGS->SpawnedMonster = MyGS->CurrentWaveIndex * EnemyCountIncrease;	// ���� ���̺� + �� ������ 
	int32 SpawnCount = MyGS->SpawnedMonster;

	// SPawnVolume ����
	AGM_SpawnVolume* SpawnVolume = 
		GetWorld()->SpawnActor<AGM_SpawnVolume>(EnemyClass, FVector::ZeroVector, FRotator::ZeroRotator);

	SpawnVolume->SpawnAICharacter(SpawnCount); // SpawnCount��ŭ �� ����
	//UE_LOG(LogTemp, Error, TEXT("SpawnCount : %d , My->SpawnedMonster : %d"), SpawnCount,MyGS->SpawnedMonster)


	

	//if (!MyGS || SpawnVolumes.Num() == 0 || !EnemyClass) return;

	//for (int32 i = 0; i < MyGS->SpawnedMonster; ++i)
	//{
	//	// ������ ���� ���� ����
	//	const int32 RandomVolumeIndex = FMath::RandRange(0, SpawnVolumes.Num() - 1);
	//	AActor* TargetVolume = SpawnVolumes[RandomVolumeIndex];

	//	// �������� BoxComponent ã��
	//	UBoxComponent* BoxComponent = TargetVolume->FindComponentByClass<UBoxComponent>();
	//	if (BoxComponent)
	//	{
	//		// BoxComponent�� ��� ������ ������ ��ġ ����
	//		FVector SpawnOrigin = BoxComponent->Bounds.Origin;
	//		FVector SpawnExtent = BoxComponent->Bounds.BoxExtent;
	//		FVector SpawnLocation = FMath::RandPointInBox(FBox(SpawnOrigin - SpawnExtent, SpawnOrigin + SpawnExtent));

	//		GetWorld()->SpawnActor<ACharacter>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
	//	}
	//}
}

// �� ���� �� ����
void AGM_GameModeBase::OnEnemyKilled()
{

	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();

	if (MyGS)
	{
		//UE_LOG(LogTemp, Error, TEXT("MyGS->SpawnedMonster : %d, MyGS->CurrentKillCount : %d "), MyGS->SpawnedMonster, MyGS->CurrentKillCount)
		if (MyGS->CurrentKillCount < MyGS->SpawnedMonster-1)		// ���� ���� �� < �� ������ �� 
		{
			MyGS->CurrentKillCount++;
		}
		else if(MyGS->CurrentKillCount == MyGS->SpawnedMonster-1)	//���� ���� ���� �� ������ ���� ������ EndWave
		{
			UE_LOG(LogTemp, Error, TEXT("Win! CurrentWave : %d"), MyGS->CurrentWaveIndex)	
			EndWave(true);
		}
	}
}

//���� ���� ó��
void AGM_GameModeBase::AddCoin(int32 Amount)
{
	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (MyGS)
	{
		MyGS->TotalCoin += Amount;	// GameState�� TotalCoin ����
	}
}

// ���� �Ծ����� ���� �� ���� ó��
void AGM_GameModeBase::OnCoinCollected()
{
	UE_LOG(LogTemp, Error, TEXT("OnCoinCollected Call!"));
	AddCoin(1);

	AGM_GameStateBase* MyGS = GetGameState<AGM_GameStateBase>();
	if (MyGS)
	{
		UE_LOG(LogTemp, Warning, TEXT("TotalCoin : %d"), MyGS->TotalCoin);
		// 5 ���θ��� ���� ����
		if (MyGS->TotalCoin > 0 && MyGS->TotalCoin % 5 == 0)
		{
			AGM_Character* MyCharacter = Cast<AGM_Character>(GetWorld()->GetFirstPlayerController()->GetPawn());
			//if (MyCharacter && MyCharacter->GetClass()->ImplementsInterface(UGM_Interface::StaticClass()))
			if(MyCharacter)
			{
				// �������̽��� ���� ĳ���Ϳ� ���� ����
				// IGM_Interface::Execute_ApplyBuff(MyCharacter, 1.5f);
				// 5���θ��� ĳ���� �ӵ� 3�� ����
				MyCharacter->UpdateMovementSpeed(MyCharacter->NormalSpeed *= 3.0f);
				UE_LOG(LogTemp, Warning, TEXT("Player buff applied! (Speed and Attack * 3.0f)"));
			}
		}
	}
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
	float Remaning = GetWorld()->GetTimerManager().GetTimerRemaining(WaveTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("Remaning Time : %f"), Remaning)	//���� �ð� ���
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
