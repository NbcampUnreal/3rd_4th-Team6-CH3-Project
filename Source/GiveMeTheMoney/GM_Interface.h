
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GM_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGM_Interface : public UInterface
{
	GENERATED_BODY()
};


class GIVEMETHEMONEY_API IGM_Interface
{
	GENERATED_BODY()

public:
	// 코인 정보를 업데이트
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void UpdateCoin(int32 TotalCoin);

	// 웨이브 정보를 업데이트
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void UpdateWave(int32 CurrentWave, int32 MaxWave);

	// 전체 게임 시간을 업데이트
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void UpdateGameTimer(float Time);

	// 현재 웨이브의 남은 시간을 업데이트
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void UpdateWaveTimer(float Time);

	// 킬 카운트를 업데이트
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void UpdateKillCount(int32 CurrentKills, int32 MinKills);

	// 게임 오버 신호
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void OnGameOver(bool bIsVictory);

	// 게임 시작 신호
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void OnGameStart();
};