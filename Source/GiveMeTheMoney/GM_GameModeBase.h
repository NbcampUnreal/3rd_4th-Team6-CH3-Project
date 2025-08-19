
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_GameModeBase.generated.h"

//���漱��
class AGM_SpawnVolume;	// ���� ���� 
class UGM_GameHUDWidget;	// UI 
class AGM_GameStateBase;
class AGM_Character;

UCLASS()
class GIVEMETHEMONEY_API AGM_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_GameModeBase();

	virtual void BeginPlay() override;

	
	//UPROPERTY(EditDefaultsOnly, Category = "SpawnVolume")
	//TSubclassOf<AGM_SpawnVolume> SpawnVolumeClass;	// AI ���� ���� Ŭ����

	UPROPERTY(EditAnywhere, Category = "SpawnVolume")
	TArray<AGM_SpawnVolume*> SpawnVolumes;	// ���� ���� �迭

	void OnCoinCollected();
	void AddCoin(int32 Amount);
	void OnEnemyKilled();
	void StartWave();
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	TSubclassOf<AGM_SpawnVolume> EnemyClass;	// AI ���� Ŭ��������

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UGM_GameHUDWidget> HUDWidgetClass;	//UI ����Ŭ���� ����

	UPROPERTY()
	TObjectPtr<UGM_GameHUDWidget> HUDWidget;	// UI �ν��Ͻ�

	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	float WaveDuration;

	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	int32 InitialEnemyCount;

	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	int32 EnemyCountIncrease;

	UPROPERTY(EditAnywhere, Category = "Game Wave")
	int32 MaxWaveIndex;

	UPROPERTY(EditAnywhere, Category = "Game Wave")
	int32 MinKillCount;

	AGM_Character* PlayerCharacter;


	void EndWave(bool bIsCompleted);
	void OnWaveTimeUp();
	void SpawnEnemies();
	void OnGameOver(bool bIsVictory);
	void UpdateHUD();

private:
	
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	AGM_GameStateBase* GetGS() const;	//GameModeBase ��ȯ
	AGM_GameStateBase* MyGS;
};
