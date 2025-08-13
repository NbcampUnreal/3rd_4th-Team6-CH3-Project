
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "GM_GameStateBase.generated.h"

/**
 *
 */
UCLASS()
class GIVEMETHEMONEY_API AGM_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AGM_GameStateBase();

	UPROPERTY(Replicated)
	int32 TotalCoin;

	UPROPERTY(Replicated)
	int32 CurrentWaveIndex;

	UPROPERTY(Replicated)
	int32 MaxWaveIndex;

	UPROPERTY(Replicated)
	float GameTime;

	UPROPERTY(Replicated)
	float WaveDuration;

	UPROPERTY(Replicated)
	int32 SpawnedMonster;

	UPROPERTY(Replicated)
	int32 CurrentKillCount;

	UPROPERTY(Replicated)
	int32 MinKillCount;
};