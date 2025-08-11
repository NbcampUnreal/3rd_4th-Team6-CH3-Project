// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GM_Controller_Character.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class GIVEMETHEMONEY_API AGM_Controller_Character : public APlayerController
{
	GENERATED_BODY()
	
public:
    AGM_Controller_Character();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")    //�Է¾׼ǰ� ������ ��𼭵� ���� �����ϰ� ����
        UInputMappingContext* InputMappingContext; // �Է� ���� ���ؽ�Ʈ (IMC)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveAction; // �̵� �׼�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* JumpAction; // ���� �׼�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* LookAction; // ī�޶�(�ü�ó��) �׼�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* SprintAction; // �̵� �׼�

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* FireAction; // �߻� �׼�

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* CrouchAction; // ��ũ���� �׼�

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* EquipWeapon1Action;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* EquipWeapon2Action;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    //TSubclassOf<UUserWidget> HUDWidgetClass;    //UI ǥ�� ����
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    //UUserWidget* HUDWidgetInstance;    // ���� �ν��Ͻ� 


    //UFUNCTION(BlueprintCallable, Category = "HUD")
    //UUserWidget* GetHUDWidget() const;    // ���� Ŭ���� ������


protected:
    virtual void BeginPlay() override; // ���� ���� �� ȣ��Ǵ� �Լ�

};
