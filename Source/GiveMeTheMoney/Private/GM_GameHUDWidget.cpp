#include "GM_GameHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "../GM_GameModeBase.h"

bool UGM_GameHUDWidget::Initialize()
{
	if (!Super::Initialize())
		return false;

	//��ư �̺�Ʈ ���ε�
	if (StartButton) StartButton->OnClicked.AddDynamic(this, &UGM_GameHUDWidget::OnStartClicked);
	if (RestartButton) RestartButton->OnClicked.AddDynamic(this, &UGM_GameHUDWidget::OnRestartClicked);
	if (QuitButton) QuitButton->OnClicked.AddDynamic(this, &UGM_GameHUDWidget::OnQuitClicked);

	return true;
}

void UGM_GameHUDWidget::ShowStartScreen()	// ���� ���� ȭ�� ǥ��
{
	if (WaveText) WaveText->SetText(FText::FromString(TEXT("Wave -")));

	UpdateTime(0);
	UpdateStats( 0);
	ShowButtons(true, false);
	//SetCenterMessage(TEXT("Game Start!"), 2.0f);
	//SetCenterMessage(TEXT("���� ���� �غ�"), 0.f);
}

void UGM_GameHUDWidget::ShowWaveStart(int32 WaveNumber)	// ���̺� ǥ�� �� ����
{
	if (WaveText) WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave %d"), WaveNumber)));
	SetCenterMessage(FString::Printf(TEXT("Wave %d Start!"), WaveNumber), 2.f);
}

void UGM_GameHUDWidget::UpdateTime(int32 Seconds)	// ���� �ð� ǥ�� ������Ʈ 
{
	if (TimeText)
	{
		int32 M = Seconds / 60;
		int32 S = Seconds % 60;
		TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), M, S)));
	}
}

void UGM_GameHUDWidget::UpdateStats(int32 Score)	//  ����(����) ǥ��
{
	// if (RemainingEnemies) RemainingEnemies->SetText(FText::FromString(FString::Printf(TEXT("Kill: %d"), KillCount)));
	if (ScoreText) ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
}

void UGM_GameHUDWidget::UpdateHP(int32 CurrentHP, int32 MaxHP)
{
	if(HPText) HPText->SetText(FText::FromString(FString::Printf(TEXT("HP : %d / %d"), CurrentHP, MaxHP)));
}

void UGM_GameHUDWidget::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (AmmoText) AmmoText->SetText(FText::FromString(FString::Printf(TEXT("Ammo : %d / %d"), CurrentAmmo, MaxAmmo)));
}

// �������� (�¸�, �й�) ����, ���� �� ǥ��
void UGM_GameHUDWidget::ShowGameOver(bool bWin, int32 FinalScore)	
{
	FString Msg = bWin ? TEXT("Win!") : TEXT("Lose...");
	Msg += FString::Printf(TEXT("\nScore: %d "), FinalScore);
	SetCenterMessage(Msg, 2.0f);	// 2�ʵ� �߾� �޼������� �� ����
	ShowButtons(false, true);
}

// �߾� �޼��� ǥ�� �����
void UGM_GameHUDWidget::ClearCentralMessage()
{
	if (CenterMessageText)
		CenterMessageText->SetText(FText::GetEmpty());
}

// ��ư ǥ�� ���� 
void UGM_GameHUDWidget::ShowButtons(bool bShowStart, bool bShowRestartQuit)
{
	if (StartButton)	
		StartButton->SetVisibility(bShowStart ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); // ESlateVisibility - ��ư ����/���� ����
	if (RestartButton)
		RestartButton->SetVisibility(bShowRestartQuit ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (QuitButton)
		QuitButton->SetVisibility(bShowRestartQuit ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}


void UGM_GameHUDWidget::SetCenterMessage(const FString& Msg, float ClearDelay)
{
	if (CenterMessageText)
		CenterMessageText->SetText(FText::FromString(Msg));

	if (ClearDelay > 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CenterMsgTimer);
		GetWorld()->GetTimerManager().SetTimer(
			CenterMsgTimer,
			[this]() { ClearCentralMessage(); },
			ClearDelay,
			false
		);
	}
}

// ���� ���� ���Ӹ�� ��ȯ
AGM_GameModeBase* UGM_GameHUDWidget::GetGM() const
{
	return Cast<AGM_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void UGM_GameHUDWidget::OnStartClicked()
{
	if (AGM_GameModeBase* GM = GetGM())
	{
		GM->StartWave();	// ���̺� ����
	}
	ShowButtons(false, false);	//��ư ����
}

// ����� 
void UGM_GameHUDWidget::OnRestartClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()));
}

void UGM_GameHUDWidget::OnQuitClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}
