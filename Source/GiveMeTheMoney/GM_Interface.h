
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GM_Interface.generated.h"

UINTERFACE(MinimalAPI)
class UGM_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class GIVEMETHEMONEY_API IGM_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Game")
	void OnGameStart();

	UFUNCTION(BlueprintNativeEvent, Category = "Game")
	void OnGameOver(bool bIsVictory);

	UFUNCTION(BlueprintNativeEvent, Category = "HUD")
	void UpdateCoin(int32 TotalCoin);

	UFUNCTION(BlueprintNativeEvent, Category = "HUD")
	void UpdateWave(int32 CurrentWave, int32 MaxWave);

	UFUNCTION(BlueprintNativeEvent, Category = "HUD")
	void UpdateGameTimer(float GameTime);

	UFUNCTION(BlueprintNativeEvent, Category = "HUD")
	void UpdateWaveTimer(float WaveTime);

	UFUNCTION(BlueprintNativeEvent, Category = "HUD")
	void UpdateKillCount(int32 KillCount, int32 MinKillCount);

	UFUNCTION(BlueprintNativeEvent, Category = "Character")
	void ApplyBuff(float Multiplier);
};
