
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_GameModeBase.generated.h"

//전방선언
class AGM_SpawnVolume;	// 스폰 볼륨 
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
	//TSubclassOf<AGM_SpawnVolume> SpawnVolumeClass;	// AI 스폰 볼륨 클래스

	UPROPERTY(EditAnywhere, Category = "SpawnVolume")
	TArray<AGM_SpawnVolume*> SpawnVolumes;	// 스폰 볼륨 배열

	void OnCoinCollected();
	void AddCoin(int32 Amount);
	void OnEnemyKilled();
	void StartWave();
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	TSubclassOf<AGM_SpawnVolume> EnemyClass;	// AI 액터 클래스저장

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UGM_GameHUDWidget> HUDWidgetClass;	//UI 위젯클래스 저장

	UPROPERTY()
	TObjectPtr<UGM_GameHUDWidget> HUDWidget;	// UI 인스턴스

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

	AGM_GameStateBase* GetGS() const;	//GameModeBase 반환
	AGM_GameStateBase* MyGS;
};
