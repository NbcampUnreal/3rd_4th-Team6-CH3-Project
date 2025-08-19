#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GM_GameHUDWidget.generated.h"

//UI�� �ʿ��� class ���漱��
class UTextBlock;
class UButton;
class UPanelWidget;
class AGM_GameModeBase;

UCLASS()
class GIVEMETHEMONEY_API UGM_GameHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;	//�ʱ�ȭ �Լ� (�����ڿ� ����ϴٰ� �����ϸ��)

	void ShowStartScreen();
	void ShowWaveStart(int32 WaveNumber);
	void UpdateTime(int32 Seconds);
	void UpdateStats(int32 Score);	// �Ű����� : int32 KillCount, ������ 
	void UpdateHP(int32 CurrentHP, int32 MaxHP);
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	void ShowGameOver(bool bWin, int32 FinalScore);
	void ClearCentralMessage();

	// ��ư �ݹ�
	UFUNCTION()
	void OnStartClicked();
	UFUNCTION()
	void OnRestartClicked();
	UFUNCTION()
	void OnQuitClicked();

	void SetCenterMessage(const FString& Msg, float ClearDelay = 0.f);	// ȭ�� �߾� �޼��� ǥ��

private:
	// ���ε��� ������ - Widget Blueprint �ȿ��� �̸� �����ϰ� ���� �ʿ�
	UPROPERTY(meta = (BindWidget)) // meta BindWidget ���� c++�� �������Ʈ ������ ����
	UTextBlock* WaveText;	// Wave ǥ��
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText;	// �ð� ǥ��
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* RemainingEnemies;	// ���� �� �� ǥ�� 
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;	// ���� ǥ��(����)
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CenterMessageText;	// �߾� �޼��� ǥ�� ( ������, ���̺� ����, �¸�, �й� ��)
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HPText;	// ���� ü�� ǥ�� 
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;	// ���� ���� ź�� ǥ�� 




	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	// ���� ���� �Լ���
	void ShowButtons(bool bShowStart, bool bShowRestartQuit);	// ��ư �����ִ� �Լ�
	

	FTimerHandle CenterMsgTimer;

	AGM_GameModeBase* GetGM() const;	// ���Ӹ�� ��ȯ
};
