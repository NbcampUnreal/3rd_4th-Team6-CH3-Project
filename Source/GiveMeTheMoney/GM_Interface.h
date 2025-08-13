
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
	// ���� ������ ������Ʈ
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void UpdateCoin(int32 TotalCoin);

	// ���̺� ������ ������Ʈ
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void UpdateWave(int32 CurrentWave, int32 MaxWave);

	// ��ü ���� �ð��� ������Ʈ
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void UpdateGameTimer(float Time);

	// ���� ���̺��� ���� �ð��� ������Ʈ
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void UpdateWaveTimer(float Time);

	// ų ī��Ʈ�� ������Ʈ
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void UpdateKillCount(int32 CurrentKills, int32 MinKills);

	// ���� ���� ��ȣ
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void OnGameOver(bool bIsVictory);

	// ���� ���� ��ȣ
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void OnGameStart();
};