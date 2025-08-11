// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Controller_Character.h"
#include "EnhancedInputSubsystems.h"
//#include "Blueprint/UserWidget.h"

AGM_Controller_Character::AGM_Controller_Character() //생성자
    : InputMappingContext(nullptr),        // 해당 입력 액션, 매핑 값은 전부 nullptr로 초기화
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    FireAction(nullptr),
    CrouchAction(nullptr),
    EquipWeapon1Action(nullptr),
    EquipWeapon2Action(nullptr)
    //HUDWidgetInstance(nullptr)
{
}

void AGM_Controller_Character::BeginPlay() // 게임 시작 시 호출되는 함수
{
    Super::BeginPlay(); // 부모 클래스의 BeginPlay 호출

    if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) // 로컬 플레이어가 존재하는지 확인 후 가져오기
    {
        if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
            LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) // LocalPlayer에 있는 SubSystem이 있는지 확인 후 가져오기
        {
            if (InputMappingContext)    //InputMapping Conext 존재시~
            {
                SubSystem->AddMappingContext(InputMappingContext, 0);    // InputMappingContext를 활서오하 시켜줌 우선순위는 0위
            }
        }
    }

    //if (HUDWidgetClass)
    //{
    //    HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);    //Widget 생성 후 HUDWidget에 할당
    //    if (HUDWidgetInstance)
    //    {
    //        HUDWidgetInstance->AddToViewport();    // 해당 위젯을 뷰포트에 표시
    //    }
    //}
}

//UUserWidget* ASpartaPlayerController::GetHUDWidget() const
//{
//    return HUDWidgetInstance;    // 유저 위젯 인스턴스 반환
//}