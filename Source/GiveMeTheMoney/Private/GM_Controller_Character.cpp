// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Controller_Character.h"
#include "EnhancedInputSubsystems.h"
//#include "Blueprint/UserWidget.h"

AGM_Controller_Character::AGM_Controller_Character() //������
    : InputMappingContext(nullptr),        // �ش� �Է� �׼�, ���� ���� ���� nullptr�� �ʱ�ȭ
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

void AGM_Controller_Character::BeginPlay() // ���� ���� �� ȣ��Ǵ� �Լ�
{
    Super::BeginPlay(); // �θ� Ŭ������ BeginPlay ȣ��

    if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) // ���� �÷��̾ �����ϴ��� Ȯ�� �� ��������
    {
        if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
            LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) // LocalPlayer�� �ִ� SubSystem�� �ִ��� Ȯ�� �� ��������
        {
            if (InputMappingContext)    //InputMapping Conext �����~
            {
                SubSystem->AddMappingContext(InputMappingContext, 0);    // InputMappingContext�� Ȱ������ ������ �켱������ 0��
            }
        }
    }

    //if (HUDWidgetClass)
    //{
    //    HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);    //Widget ���� �� HUDWidget�� �Ҵ�
    //    if (HUDWidgetInstance)
    //    {
    //        HUDWidgetInstance->AddToViewport();    // �ش� ������ ����Ʈ�� ǥ��
    //    }
    //}
}

//UUserWidget* ASpartaPlayerController::GetHUDWidget() const
//{
//    return HUDWidgetInstance;    // ���� ���� �ν��Ͻ� ��ȯ
//}