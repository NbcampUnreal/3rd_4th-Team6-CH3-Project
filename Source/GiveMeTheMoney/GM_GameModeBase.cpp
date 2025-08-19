
#include "GM_GameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "GM_GameStateBase.h"
#include "GM_Character.h"
#include "GM_Controller_Character.h"
#include "GM_Interface.h"
#include "GM_SpawnVolume.h"
#include "GM_GameHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

AGM_GameModeBase::AGM_GameModeBase()
{
	// 기본값 설정
	EnemyClass = nullptr;
	WaveDuration = 30.0f;
	InitialEnemyCount = 10;
	EnemyCountIncrease = 5;
	MaxWaveIndex = 5;
	MinKillCount = 10;
	
}

void AGM_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AGM_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	// GameState 초기화
	 MyGS = GetGS();
	if (MyGS)
	{
		MyGS->MaxWaveIndex = MaxWaveIndex;
		MyGS->MinKillCount = MinKillCount;

		// GameState를 통해 모든 클라이언트에게 게임 시작 신호를 보냅니다.
		if (MyGS->GetClass()->ImplementsInterface(UGM_Interface::StaticClass()))
		{
			IGM_Interface::Execute_OnGameStart(MyGS);
		}
	}

	
	// 시작 메뉴 위젯 표시
	if (HUDWidgetClass)	
	{
		HUDWidget = CreateWidget<UGM_GameHUDWidget>(GetWorld(), HUDWidgetClass);	// 위젯 생성
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();		// 뷰포트 표시
			HUDWidget->ShowStartScreen();	// 시작 화면 표시
			HUDWidget->UpdateHP(PlayerCharacter->GetHealth(), 500);
		}
	}

	
	

	// Input Mode 설정
	if (AGM_Controller_Character* PlayerController = Cast<AGM_Controller_Character>(GetWorld()->GetFirstPlayerController()))
	{
		FInputModeUIOnly InputMode;	// Input 모드 구조체 선언
		InputMode.SetWidgetToFocus(HUDWidget->TakeWidget());		// 위젯 포커스 설정
		PlayerController->SetInputMode(InputMode);				// UI 에만 입력가능하도록 설정
		PlayerController->SetShowMouseCursor(true);				// 마우스 표시
	}


	
}

void AGM_GameModeBase::StartWave()
{
	if (!MyGS) return;

	//UE_LOG(LogTemp, Warning, TEXT("Start Wave!, CurrentWave : %d"), MyGS->CurrentWaveIndex)	// 현재웨이브 로그 출력
	
	// 현재 웨이브 조건들 초기화 (현재 킬수 웨이브 남은 시간)
	MyGS->WaveDuration = WaveDuration;
	MyGS->CurrentKillCount = 0;
	


	//AI 캐릭터 스폰하기		
	SpawnEnemies();	

	// HUD 업데이트 타이머 시작 (1초마다)
	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &AGM_GameModeBase::UpdateHUD, 0.2f, true);
	// 웨이브 시간 타이머 시작
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AGM_GameModeBase::OnWaveTimeUp, MyGS->WaveDuration, false);

	if (HUDWidget)
	{
		HUDWidget->ShowWaveStart(MyGS->CurrentWaveIndex);	// 웨이브 시작 메세지 중앙 표시
		
	}

	// Input Mode 를 Game Only로 변경
	if (AGM_Controller_Character* PlayerController = Cast<AGM_Controller_Character>(GetWorld()->GetFirstPlayerController()))
	{
		FInputModeGameOnly InputMode;	// Input 모드 구조체 선언
		PlayerController->SetInputMode(InputMode);				// UI 에만 입력가능하도록 설정
		PlayerController->SetShowMouseCursor(false);				// 마우스 표시
	}

	MyGS->CurrentWaveIndex++;	//  웨이브 인덱스 증가
}

void AGM_GameModeBase::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (HUDWidget)
	{
		HUDWidget->UpdateAmmo(PlayerCharacter->CurrentWeapon->CurrentAmmo, PlayerCharacter->CurrentWeapon->MaxAmmo);
	}
}

void AGM_GameModeBase::EndWave(bool bIsCompleted)
{
	// 타이머 핸들러 초기화
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);

	if (!MyGS) return;

	// 웨이브 성공/실패 조건 확인
	if (bIsCompleted && MyGS->CurrentKillCount >= MyGS->MinKillCount)
	{
		// 마지막 웨이브였는지 확인
		if (MyGS->CurrentWaveIndex > MyGS->MaxWaveIndex)
		{
			OnGameOver(true); // 게임 승리
		}
		else
		{
			StartWave(); // 다음 웨이브 시작
		}
	}
	else
	{
		OnGameOver(false); // 게임 오버
	}
}

void AGM_GameModeBase::OnWaveTimeUp()
{
	// AI 잡은 후 최소 AI 캐릭 수 이상 웨이브 시간 종료시 생존 후 다음 웨이브 호출
	if (!MyGS) return;

	if (MyGS->CurrentKillCount >= MyGS->MinKillCount)	// Kill 수가 최소 킬수 이상일때 true
	{
		EndWave(true);
	}
	else
	{
		EndWave(false); // 시간이 다 되면 웨이브 실패
	}
}

void AGM_GameModeBase::SpawnEnemies()
{
	if (!MyGS || EnemyClass == nullptr) return;	// GameState 없고 EnemyClass 없을때 실행 X

	MyGS->SpawnedMonster = MyGS->CurrentWaveIndex * EnemyCountIncrease;	// 현재 웨이브 + 적 증가량 
	int32 SpawnCount = MyGS->SpawnedMonster;

	// SPawnVolume 스폰
	AGM_SpawnVolume* SpawnVolume = 
		GetWorld()->SpawnActor<AGM_SpawnVolume>(EnemyClass, FVector::ZeroVector, FRotator::ZeroRotator);

	SpawnVolume->SpawnAICharacter(SpawnCount); // SpawnCount만큼 적 생성
	//UE_LOG(LogTemp, Error, TEXT("SpawnCount : %d , My->SpawnedMonster : %d"), SpawnCount,MyGS->SpawnedMonster)
}

// 적 잡은 수 증가
void AGM_GameModeBase::OnEnemyKilled()
{

	if (MyGS)
	{
		//UE_LOG(LogTemp, Error, TEXT("MyGS->SpawnedMonster : %d, MyGS->CurrentKillCount : %d "), MyGS->SpawnedMonster, MyGS->CurrentKillCount)
		if (MyGS->CurrentKillCount < MyGS->SpawnedMonster-1)		// 현재 죽인 수 < 적 스폰된 수 
		{
			MyGS->CurrentKillCount++;
		}
		else if(MyGS->CurrentKillCount == MyGS->SpawnedMonster-1)	//현재 죽인 수와 적 스폰된 수가 같을때 EndWave
		{
			UE_LOG(LogTemp, Error, TEXT("Win! CurrentWave : %d"), MyGS->CurrentWaveIndex)	
			EndWave(true);
		}
	}
}

//코인 증가 처리
void AGM_GameModeBase::AddCoin(int32 Amount)
{
	if (MyGS)
	{
		MyGS->TotalCoin += Amount;	// GameState의 TotalCoin 증가
		if (HUDWidget)
		{
			HUDWidget->UpdateStats(MyGS->TotalCoin);	// 위젯 업데이트(Kill, Coin 표시)
		}
	}
}

// 코인 먹었을때 증가 및 버프 처리
void AGM_GameModeBase::OnCoinCollected()
{
	UE_LOG(LogTemp, Error, TEXT("OnCoinCollected Call!"));
	AddCoin(1);

	if (MyGS)
	{
		UE_LOG(LogTemp, Warning, TEXT("TotalCoin : %d"), MyGS->TotalCoin);
		// 5 코인마다 버프 적용
		if (MyGS->TotalCoin > 0 && MyGS->TotalCoin % 5 == 0)
		{
			AGM_Character* MyCharacter = Cast<AGM_Character>(GetWorld()->GetFirstPlayerController()->GetPawn());
			//if (MyCharacter && MyCharacter->GetClass()->ImplementsInterface(UGM_Interface::StaticClass()))
			if(MyCharacter)
			{
				// 인터페이스를 통해 캐릭터에 버프 적용
				// IGM_Interface::Execute_ApplyBuff(MyCharacter, 1.5f);
				// 5코인마다 캐릭터 속도 3배 증가
				MyCharacter->UpdateMovementSpeed(MyCharacter->NormalSpeed *= 3.0f);
				UE_LOG(LogTemp, Warning, TEXT("Player buff applied! (Speed and Attack * 3.0f)"));
			}
		}
	}
}

void AGM_GameModeBase::OnGameOver(bool bIsVictory)
{
	// 모든 타이머 중지
	GetWorldTimerManager().ClearAllTimersForObject(this);

	PlayerCharacter->StopMovement();

	// Input Mode 설정
	if (AGM_Controller_Character* PlayerController = Cast<AGM_Controller_Character>(GetWorld()->GetFirstPlayerController()))
	{
		FInputModeUIOnly InputMode;	// Input 모드 구조체 선언
		InputMode.SetWidgetToFocus(HUDWidget->TakeWidget());		// 위젯 포커스 설정
		PlayerController->SetInputMode(InputMode);				// UI 에만 입력가능하도록 설정
		PlayerController->SetShowMouseCursor(true);				// 마우스 표시
	}

	if (bIsVictory)
	{
		UE_LOG(LogTemp, Log, TEXT("Game Won!"));
		if (HUDWidget)
		{
			HUDWidget->ShowGameOver(true, MyGS->TotalCoin);	// 게임종료 후 코인갯수 표시
		}

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Game Over!"));
		if (HUDWidget)
		{
			HUDWidget->ShowGameOver(false, MyGS->TotalCoin);
		}
	}

	// GameState를 통해 모든 클라이언트에게 게임 오버 신호를 보냅니다.
	if (MyGS && MyGS->GetClass()->ImplementsInterface(UGM_Interface::StaticClass()))
	{
		IGM_Interface::Execute_OnGameOver(MyGS, bIsVictory);
	}

	//게임 오버 관련 로직 추가
}

void AGM_GameModeBase::UpdateHUD()
{
	float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(WaveTimerHandle); // 남은 시간

	if(HUDWidget)
	{ 
		HUDWidget->UpdateTime(static_cast<int32>(RemainingTime));	// 남은시간 위젯 표시
	}

}

AGM_GameStateBase* AGM_GameModeBase::GetGS() const
{
	return GetGameState<AGM_GameStateBase>();
}
