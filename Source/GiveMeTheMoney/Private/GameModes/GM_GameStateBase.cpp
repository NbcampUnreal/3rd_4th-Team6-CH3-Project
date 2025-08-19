
#include "GameModes/GM_GameStateBase.h"
#include "Net/UnrealNetwork.h"

AGM_GameStateBase::AGM_GameStateBase()
{
	TotalCoin = 0;
	CurrentCoin = 0;
	CurrentWaveIndex = 1;
	MaxWaveIndex = 5; // 예시 최대 웨이브
	GameTime = 0.0f;
	WaveDuration = 20.0f;	// 웨이브 시간
	SpawnedMonster = 0;
	CurrentKillCount = 0;
	MinKillCount = 10; // 예시 최소 킬 수
}

void AGM_GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGM_GameStateBase, TotalCoin);
	DOREPLIFETIME(AGM_GameStateBase, CurrentWaveIndex);
	DOREPLIFETIME(AGM_GameStateBase, MaxWaveIndex);
	DOREPLIFETIME(AGM_GameStateBase, GameTime);
	DOREPLIFETIME(AGM_GameStateBase, WaveDuration);
	DOREPLIFETIME(AGM_GameStateBase, SpawnedMonster);
	DOREPLIFETIME(AGM_GameStateBase, CurrentKillCount);
	DOREPLIFETIME(AGM_GameStateBase, MinKillCount);
}
