// ���� �ÿ��� ���� ĳ���� �÷��� ��Ʈ�ѷ� ���
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* ReloadAction;
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    //TSubclassOf<UUserWidget> HUDWidgetClass;    //UI ǥ�� ����
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    //UUserWidget* HUDWidgetInstance;    // ���� �ν��Ͻ� 


    //UFUNCTION(BlueprintCallable, Category = "HUD")
    //UUserWidget* GetHUDWidget() const;    // ���� Ŭ���� ������


protected:
    virtual void BeginPlay() override; // ���� ���� �� ȣ��Ǵ� �Լ�

};
