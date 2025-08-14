// 무기 시연용 더미 캐릭터 플레이 컨트롤러 헤더
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GM_Dummy_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class GIVEMETHEMONEY_API AGM_Dummy_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public : 
	AGM_Dummy_PlayerController();

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputMappingContext* InputMappingContext;	// 입력 매핑 컨텍스트
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* MoveAction;	// 이동 액션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* LookAction;	// 시점 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* JumpAction;	// 점프 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* ShotAction;	// 무기 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* ReloadAction;	// 재장전 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* ChangeWeaponOneAction;	// 1번 무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* ChangeWeaponTwoAction;	// 2번 무기
};
