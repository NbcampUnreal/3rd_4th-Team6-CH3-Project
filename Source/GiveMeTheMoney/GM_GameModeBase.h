
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
	// 게임 시작 시 자동으로 호출되는 함수
	virtual void BeginPlay() override;

private:
	// --- 게임 규칙 설정 (블루프린트에서 수정 가능) ---
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

	// --- 내부 변수 ---
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
	TArray<AActor*> SpawnVolumes;

	// --- 내부 함수 ---
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