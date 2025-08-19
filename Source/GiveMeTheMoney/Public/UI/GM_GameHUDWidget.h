#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GM_GameHUDWidget.generated.h"

//UI에 필요한 class 전방선언
class UTextBlock;
class UButton;
class UPanelWidget;
class AGM_GameModeBase;

UCLASS()
class GIVEMETHEMONEY_API UGM_GameHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;	//초기화 함수 (생성자와 비슷하다고 생각하면됨)

	void ShowStartScreen();
	void ShowWaveStart(int32 WaveNumber);
	void UpdateTime(int32 Seconds);
	void UpdateStats(int32 Score);	// 매개변수 : int32 KillCount, 지웠음 
	void UpdateHP(int32 CurrentHP, int32 MaxHP);
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	void ShowGameOver(bool bWin, int32 FinalScore);
	void ClearCentralMessage();

	// 버튼 콜백
	UFUNCTION()
	void OnStartClicked();
	UFUNCTION()
	void OnRestartClicked();
	UFUNCTION()
	void OnQuitClicked();

	void SetCenterMessage(const FString& Msg, float ClearDelay = 0.f);	// 화면 중앙 메세지 표시

private:
	// 바인딩될 위젯들 - Widget Blueprint 안에서 이름 동일하게 설정 필요
	UPROPERTY(meta = (BindWidget)) // meta BindWidget 으로 c++과 블루프린트 위젯을 연결
	UTextBlock* WaveText;	// Wave 표시
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText;	// 시간 표시
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* RemainingEnemies;	// 남은 적 수 표시 
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;	// 점수 표시(코인)
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CenterMessageText;	// 중앙 메세지 표시 ( 재장전, 웨이브 시작, 승리, 패배 등)
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HPText;	// 현재 체력 표시 
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;	// 현재 남은 탄약 표시 




	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	// 내부 헬퍼 함수들
	void ShowButtons(bool bShowStart, bool bShowRestartQuit);	// 버튼 보여주는 함수
	

	FTimerHandle CenterMsgTimer;

	AGM_GameModeBase* GetGM() const;	// 게임모드 반환
};
