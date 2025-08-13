
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_GameModeBase.generated.h"

UCLASS()
class GIVEMETHEMONEY_API AGM_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_GameModeBase();

protected:
	// ���� ���� �� �ڵ����� ȣ��Ǵ� �Լ�
	virtual void BeginPlay() override;

private:
	// --- ���� ��Ģ ���� (�������Ʈ���� ���� ����) ---
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	TSubclassOf<class ACharacter> EnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float WaveDuration;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 InitialEnemyCount;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 EnemyCountIncrease;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 MaxWaveIndex;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 MinKillCount;

	// --- ���� ���� ---
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
	TArray<AActor*> SpawnVolumes;

	// --- ���� �Լ� ---
	void StartWave();
	void EndWave(bool bIsCompleted);
	void OnWaveTimeUp();
	void SpawnEnemies();

	void AddCoin(int32 Amount);
	void OnEnemyKilled();
	void OnCoinCollected();

	void OnGameOver(bool bIsVictory);
	void UpdateHUD();
};