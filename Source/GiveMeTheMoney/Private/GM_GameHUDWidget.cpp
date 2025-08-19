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

	//버튼 이벤트 바인딩
	if (StartButton) StartButton->OnClicked.AddDynamic(this, &UGM_GameHUDWidget::OnStartClicked);
	if (RestartButton) RestartButton->OnClicked.AddDynamic(this, &UGM_GameHUDWidget::OnRestartClicked);
	if (QuitButton) QuitButton->OnClicked.AddDynamic(this, &UGM_GameHUDWidget::OnQuitClicked);

	return true;
}

void UGM_GameHUDWidget::ShowStartScreen()	// 게임 시작 화면 표시
{
	if (WaveText) WaveText->SetText(FText::FromString(TEXT("Wave -")));

	UpdateTime(0);
	UpdateStats( 0);
	ShowButtons(true, false);
	//SetCenterMessage(TEXT("Game Start!"), 2.0f);
	//SetCenterMessage(TEXT("게임 시작 준비"), 0.f);
}

void UGM_GameHUDWidget::ShowWaveStart(int32 WaveNumber)	// 웨이브 표시 및 시작
{
	if (WaveText) WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave %d"), WaveNumber)));
	SetCenterMessage(FString::Printf(TEXT("Wave %d Start!"), WaveNumber), 2.f);
}

void UGM_GameHUDWidget::UpdateTime(int32 Seconds)	// 게임 시간 표시 업데이트 
{
	if (TimeText)
	{
		int32 M = Seconds / 60;
		int32 S = Seconds % 60;
		TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), M, S)));
	}
}

void UGM_GameHUDWidget::UpdateStats(int32 Score)	//  점수(코인) 표시
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

// 게임종료 (승리, 패배) 점수, 죽인 수 표시
void UGM_GameHUDWidget::ShowGameOver(bool bWin, int32 FinalScore)	
{
	FString Msg = bWin ? TEXT("Win!") : TEXT("Lose...");
	Msg += FString::Printf(TEXT("\nScore: %d "), FinalScore);
	SetCenterMessage(Msg, 2.0f);	// 2초뒤 중앙 메세지설정 후 제거
	ShowButtons(false, true);
}

// 중앙 메세지 표시 지우기
void UGM_GameHUDWidget::ClearCentralMessage()
{
	if (CenterMessageText)
		CenterMessageText->SetText(FText::GetEmpty());
}

// 버튼 표시 설정 
void UGM_GameHUDWidget::ShowButtons(bool bShowStart, bool bShowRestartQuit)
{
	if (StartButton)	
		StartButton->SetVisibility(bShowStart ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); // ESlateVisibility - 버튼 숨김/보임 상태
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

// 현재 레벨 게임모드 반환
AGM_GameModeBase* UGM_GameHUDWidget::GetGM() const
{
	return Cast<AGM_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void UGM_GameHUDWidget::OnStartClicked()
{
	if (AGM_GameModeBase* GM = GetGM())
	{
		GM->StartWave();	// 웨이브 시작
	}
	ShowButtons(false, false);	//버튼 제거
}

// 재시작 
void UGM_GameHUDWidget::OnRestartClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()));
}

void UGM_GameHUDWidget::OnQuitClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}
