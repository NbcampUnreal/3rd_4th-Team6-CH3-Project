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

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputMappingContext* InputMappingContext;	// �Է� ���� ���ؽ�Ʈ
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* MoveAction;	// �̵� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* LookAction;	// ���� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* JumpAction;	// ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* ShotAction;	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* ReloadAction;	// ������ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* ChangeWeaponOneAction;	// 1�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DummyPlayerController")
	UInputAction* ChangeWeaponTwoAction;	// 2�� ����
};
