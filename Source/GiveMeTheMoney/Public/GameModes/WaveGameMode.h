// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WaveGameMode.generated.h"

class ASpawnVolume;
class AMonster;
class UWaveHUDWidget;

// 1웨이브 - 적이 1명, 스폰 간격 - 1초, 웨이브 제한 시간 60초다
// 2웨이브 - 적이 2명, 스폰 간격 - 1초, 웨이브 제한 시간 50초다
USTRUCT(BlueprintType)
struct FWaveConfig // 웨이브
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 EnemyCount = 0; // 적 카운트

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float SpawnInterval = 1.0f; // 적이 스폰되는 간격(시간)

	// 웨이브 마다 제한시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TimeLimitSeconds = 60;
};

UCLASS()
class MONSTERSHOOTER2_API AGM_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_GameModeBase();

protected:
	virtual void BeginPlay() override;

	// 웨이브 설정 배열
	UPROPERTY(EditAnywhere, Category = "Wave")
	TArray<FWaveConfig> Waves; // [ FWaveConfig, FWaveConfig, FWaveConfig ] -> 이건 언리얼 에디터에서 설정한다.

	// 현재 웨이브 -1은 시작 안함!
	UPROPERTY(VisibleAnywhere, Category = "Wave")
	int32 CurrentWaveIndex = -1;

	// 살아있는 적 수
	UPROPERTY(VisibleAnywhere, Category = "Wave")
	int32 AliveEnemies = 0;

	// 스폰해야 할 남은 적
	UPROPERTY(VisibleAnywhere, Category = "Wave")
	int32 RemainingToSpawn = 0;

	// 스폰할 몬스터 클래스 - Blueprint에서 지정
	UPROPERTY(EditAnywhere, Category = "Wave")
	TSubclassOf<AMonster> MonsterClass; // 스폰해야할 몬스터도 에디터의 BP_WaveGameMode에서 설정한다.

	// 스폰 영역
	UPROPERTY()
	TArray<TObjectPtr<ASpawnVolume>> SpawnVolumes;

	// 게임 제한 시간
	UPROPERTY(EditAnywhere, Category = "Game")
	int32 TotalTimeSeconds = 60; // 현재 사용하지 않음 -> FWaveConfig 안에 있는 타임 사용 중

	// 남은 시간
	UPROPERTY(VisibleAnywhere, Category = "Game")
	int32 RemainingTimeSeconds = 0;

	// 점수 , 킬
	UPROPERTY(VisibleAnywhere, Category = "Score")
	int32 KillCount = 0;

	UPROPERTY(VisibleAnywhere, Category = "Score")
	int32 Score = 0;

	// 타이머 핸들
	FTimerHandle WaveSpawnTimerHandle;
	FTimerHandle GameTimerHandle;

	// 필요한 함수들
	void StartNextWave();
	void SpawnOneEnemy();
	void CheckWaveEnd();
	void TickGameTimer();
	void GameOver(bool bVictory);

	// 랜덤 스폰 위치 선택
	bool GetRandomSpawnLocation(FVector& OutLocation) const;

	// 위젯용 변수들
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass; // 설계도

	UPROPERTY()
	UWaveHUDWidget* HUDWidget = nullptr; // 완성된 위젯

public:
	void StartGame(); // 다른쪽에서 호출 하기 위해 public에 위치

	// 상태 플래그 - 게임이 실행중인지 아닌지
	bool bGameActive = false;

	// 몬스터가 죽을 때 호출 - MonsterAI에서
	void ReportEnemyDeath();

	// UI용 Getter
	// int32 GetCurrentWave() const { return CurrentWaveIndex + 1; }
	// int32 GetRemainingTime() const { return RemainingTimeSeconds; }
	// int32 GetKillCount() const { return KillCount; }
	// int32 GetScore() const { return Score; }
};
