
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_GameModeBase.generated.h"

class AGM_SpawnVolume;	// 스폰 볼륨 전방선언

UCLASS()
class GIVEMETHEMONEY_API AGM_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_GameModeBase();

	virtual void BeginPlay() override;

	
	//UPROPERTY(EditDefaultsOnly, Category = "SpawnVolume")
	//TSubclassOf<AGM_SpawnVolume> SpawnVolumeClass;	// AI 스폰 볼륨 클래스

	UPROPERTY(EditAnywhere, Category = "SpawnVolume")
	TArray<AGM_SpawnVolume*> SpawnVolumes;	// 스폰 볼륨 배열

	void OnCoinCollected();
	void AddCoin(int32 Amount);
	void OnEnemyKilled();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	TSubclassOf<AGM_SpawnVolume> EnemyClass;	// AI 액터 클래스저장

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

	void StartWave();
	void EndWave(bool bIsCompleted);
	void OnWaveTimeUp();
	void SpawnEnemies();
	void OnGameOver(bool bIsVictory);
	void UpdateHUD();

private:
	
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
};
