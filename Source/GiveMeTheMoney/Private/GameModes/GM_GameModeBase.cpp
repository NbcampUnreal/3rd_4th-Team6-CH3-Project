
#include "GameModes/GM_GameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "GameModes/GM_GameStateBase.h"
#include "Character/GM_Character.h"
#include "Character/GM_Controller_Character.h"
#include "GM_SpawnVolume.h"
#include "UI/GM_GameHUDWidget.h"
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

	//ĳ����, ��Ʈ�ѷ� ��������
	PlayerCharacter = Cast<AGM_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	PC = Cast< AGM_Controller_Character>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// GameState �ʱ�ȭ
	 MyGS = GetGS();
	if (MyGS)
	{
		MyGS->MaxWaveIndex = MaxWaveIndex;
		MyGS->MinKillCount = MinKillCount;
	}

	// ���� �޴� ���� ǥ��
	if (HUDWidgetClass)	
	{
		HUDWidget = CreateWidget<UGM_GameHUDWidget>(GetWorld(), HUDWidgetClass);	// ���� ����
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();		// ����Ʈ ǥ��
			HUDWidget->ShowStartScreen();	// ���� ȭ�� ǥ��
			HUDWidget->UpdateHP(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());	// ü�� ǥ��
		}
	}

	// Input Mode ����
	if (PC)
	{
		FInputModeUIOnly InputMode;	// Input ��� ����ü ����
		InputMode.SetWidgetToFocus(HUDWidget->TakeWidget());		// ���� ��Ŀ�� ����
		PC->SetInputMode(InputMode);				// UI ���� �Է°����ϵ��� ����
		PC->SetShowMouseCursor(true);				// ���콺 ǥ��
	}	
}

void AGM_GameModeBase::StartWave()
{
	if (!MyGS) return;

	//UE_LOG(LogTemp, Warning, TEXT("Start Wave!, CurrentWave : %d"), MyGS->CurrentWaveIndex)	// ������̺� �α� ���

	EnablePlayerControl();	// ĳ���� �Է� ����

	// ���� ���̺� ���ǵ� �ʱ�ȭ (���� ų�� ���̺� ���� �ð�)
	MyGS->WaveDuration = WaveDuration;
	MyGS->CurrentKillCount = 0;
	MyGS->CurrentCoin = 0;	// CurrentCoin �ʱ�ȭ
	ResetCharacterSpeed();	// ������ �ӵ� �ʱ�ȭ
	
	//AI ĳ���� �����ϱ�		
	SpawnEnemies();	

	// HUD ������Ʈ Ÿ�̸� ���� (0.1�ʸ���)
	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &AGM_GameModeBase::UpdateHUD, 0.1f, true);
	// ���̺� �ð� Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AGM_GameModeBase::OnWaveTimeUp, MyGS->WaveDuration, false);

	if (HUDWidget)
	{
		HUDWidget->ShowWaveStart(MyGS->CurrentWaveIndex);	// ���̺� ���� �޼��� �߾� ǥ��
		
	}

	// Input Mode �� Game Only�� ����
	if (PC)
	{
		FInputModeGameOnly InputMode;	// Input ��� ����ü ����
		PC->SetInputMode(InputMode);				// UI ���� �Է°����ϵ��� ����
		PC->SetShowMouseCursor(false);				// ���콺 ǥ��
	}

	MyGS->CurrentWaveIndex++;	//  ���̺� �ε��� ����
}

void AGM_GameModeBase::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (HUDWidget)
	{
		HUDWidget->UpdateAmmo(PlayerCharacter->CurrentWeapon->CurrentAmmo, PlayerCharacter->CurrentWeapon->MaxAmmo);
	}
}

void AGM_GameModeBase::ResetCharacterSpeed()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->UpdateMovementSpeed(PlayerCharacter->NormalSpeed);
	}
}

void AGM_GameModeBase::EndWave(bool bIsCompleted)
{
	// Ÿ�̸� �ڵ鷯 �ʱ�ȭ
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	//ResetCharacterSpeed();	// Wave ���� �� Speed �ʱ�ȭ
	
	if (!MyGS) return;

	// ���̺� ����/���� ���� Ȯ��
	if (bIsCompleted && MyGS->CurrentKillCount >= MyGS->MinKillCount)
	{
		// ������ ���̺꿴���� Ȯ��
		if (MyGS->CurrentWaveIndex > MyGS->MaxWaveIndex)
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
	// AI ���� �� �ּ� AI ĳ�� �� �̻� ���̺� �ð� ����� ���� �� ���� ���̺� ȣ��
	if (!MyGS) return;

	if (MyGS->CurrentKillCount >= MyGS->MinKillCount)	// Kill ���� �ּ� ų�� �̻��϶� true
	{
		EndWave(true);
	}
	else
	{
		EndWave(false); // �ð��� �� �Ǹ� ���̺� ����
	}
}

void AGM_GameModeBase::SpawnEnemies()
{
	if (!MyGS || EnemyClass == nullptr) return;	// GameState ���� EnemyClass ������ ���� X

	MyGS->SpawnedMonster = MyGS->CurrentWaveIndex * EnemyCountIncrease;	// ���� ���̺� + �� ������ 
	int32 SpawnCount = MyGS->SpawnedMonster;

	// SPawnVolume ����
	AGM_SpawnVolume* SpawnVolume = 
		GetWorld()->SpawnActor<AGM_SpawnVolume>(EnemyClass, FVector::ZeroVector, FRotator::ZeroRotator);

	SpawnVolume->SpawnAICharacter(SpawnCount); // SpawnCount��ŭ �� ����
}

// �� ���� �� ����
void AGM_GameModeBase::OnEnemyKilled()
{
	if (MyGS)
	{
		//UE_LOG(LogTemp, Error, TEXT("MyGS->SpawnedMonster : %d, MyGS->CurrentKillCount : %d "), MyGS->SpawnedMonster, MyGS->CurrentKillCount)
		if (MyGS->CurrentKillCount < MyGS->SpawnedMonster-1)		// ���� ���� �� < �� ������ �� 
		{
			MyGS->CurrentKillCount++;
		}
		else if(MyGS->CurrentKillCount == MyGS->SpawnedMonster-1)	//���� ���� ���� �� ������ ���� ������ EndWave
		{
			//UE_LOG(LogTemp, Error, TEXT("Win! CurrentWave : %d"), MyGS->CurrentWaveIndex)	
			EndWave(true);
		}
	}
}

//���� ���� ó��
void AGM_GameModeBase::AddCoin(int32 Amount)
{
	if (MyGS)
	{
		MyGS->CurrentCoin += Amount;	// GameState�� CurrentCoin ���� �� Total ����
		MyGS->TotalCoin += Amount;
		if (HUDWidget)
		{
			HUDWidget->UpdateStats(MyGS->TotalCoin);	// ���� ������Ʈ(Kill, Coin ǥ��)
		}
	}
}

// ���� �Ծ����� ���� �� ���� ó��
void AGM_GameModeBase::OnCoinCollected()
{
	UE_LOG(LogTemp, Error, TEXT("OnCoinCollected Call!"));
	AddCoin(1);

	if (MyGS)
	{
		// UE_LOG(LogTemp, Warning, TEXT("TotalCoin : %d"), MyGS->TotalCoin);
		// 5 ���θ��� ���� ����
		if (MyGS->CurrentCoin > 0 && MyGS->CurrentCoin % 5 == 0)
		{
			if(PlayerCharacter)
			{
				// 5���θ��� ĳ���� �ӵ� 3�� ����
				PlayerCharacter->UpdateMovementSpeed(PlayerCharacter->NormalSpeed * 3.0f);
				//UE_LOG(LogTemp, Warning, TEXT("Player buff applied! (Speed and Attack * 3.0f)"));
			}
		}
	}
}

void AGM_GameModeBase::OnGameOver(bool bIsVictory)
{
	// ��� Ÿ�̸� ����
	GetWorldTimerManager().ClearAllTimersForObject(this);


	//UE_LOG(LogTemp, Warning, TEXT("StopMovement Called"));

	// Input Mode ����
	if (PC)
	{
		FInputModeUIOnly InputMode;	// Input ��� ����ü ����
		InputMode.SetWidgetToFocus(HUDWidget->TakeWidget());		// ���� ��Ŀ�� ����
		PC->SetInputMode(InputMode);				// UI ���� �Է°����ϵ��� ����
		DisablePlayerControl();	// ĳ���� �Է� ���� �� �̵� ����

		//PC->DisableInput(PC);		// ĳ���� �Է� ����
		//PC->SetShowMouseCursor(true);				// ���콺 ǥ��

	}

	if (bIsVictory)
	{
		UE_LOG(LogTemp, Log, TEXT("Game Won!"));
		if (HUDWidget)
		{
			HUDWidget->ShowGameOver(true, MyGS->TotalCoin);	// �������� �� ���ΰ��� ǥ��
		}

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Game Over!"));
		if (HUDWidget)
		{
			HUDWidget->ShowGameOver(false, MyGS->TotalCoin);
		}
	}
}

void AGM_GameModeBase::UpdateHUD()
{
	float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(WaveTimerHandle); // ���� �ð�

	if(HUDWidget)
	{ 
		HUDWidget->UpdateTime(static_cast<int32>(RemainingTime));	// �����ð� ���� ǥ��
	}

}

// ĳ���� �Է� ����
void AGM_GameModeBase::DisablePlayerControl()
{
	if (PC)
	{
		PC->DisableInput(PC);
		PC->SetShowMouseCursor(true);

		if (PlayerCharacter)
		{
			PlayerCharacter->StopMovement(true);	//ĳ���� �̵� X
		}
	}
}
// �Է� Ȱ��ȭ
void AGM_GameModeBase::EnablePlayerControl()
{
	if (PC)
	{
		PC->EnableInput(PC);
		PC->SetShowMouseCursor(false);

		if (PlayerCharacter)
		{
			PlayerCharacter->StopMovement(false);	// ĳ���� �̵� ����
		}
	}
}

AGM_GameStateBase* AGM_GameModeBase::GetGS() const
{
	return GetGameState<AGM_GameStateBase>();
}
