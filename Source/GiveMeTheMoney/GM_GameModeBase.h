
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_GameModeBase.generated.h"

/**
 *
 */
UCLASS()
class GIVEMETHEMONEY_API AGM_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_GameModeBase();

	virtual void BeginPlay() override;

	void OnCoinCollected();
	void AddCoin(int32 Amount);
	void OnEnemyKilled();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	TSubclassOf<ACharacter> EnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	float WaveDuration;

	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	int32 InitialEnemyCount;

	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	int32 EnemyCountIncrease;

	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	int32 MaxWaveIndex;

	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	int32 MinKillCount;

	void StartWave();
	void EndWave(bool bIsCompleted);
	void OnWaveTimeUp();
	void SpawnEnemies();
	void OnGameOver(bool bIsVictory);
	void UpdateHUD();

private:
	TArray<AActor*> SpawnVolumes;
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
};
