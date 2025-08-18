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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")    //입력액션과 매핑은 어디서든 수정 가능하게 설정
        UInputMappingContext* InputMappingContext; // 입력 매핑 컨텍스트 (IMC)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveAction; // 이동 액션
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* JumpAction; // 점프 액션
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* LookAction; // 카메라(시선처리) 액션
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* SprintAction; // 이동 액션

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* FireAction; // 발사 액션

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* CrouchAction; // 웅크리기 액션

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* EquipWeapon1Action;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* EquipWeapon2Action;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* ReloadAction;
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    //TSubclassOf<UUserWidget> HUDWidgetClass;    //UI 표시 위젯
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    //UUserWidget* HUDWidgetInstance;    // 위젯 인스턴스 


    //UFUNCTION(BlueprintCallable, Category = "HUD")
    //UUserWidget* GetHUDWidget() const;    // 위젯 클래스 얻어오기


protected:
    virtual void BeginPlay() override; // 게임 시작 시 호출되는 함수

};
