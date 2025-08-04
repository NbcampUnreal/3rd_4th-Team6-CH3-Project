//�ÿ��� ���� ĳ���� ���
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "GameFramework/CharacterMovementComponent.h"
#include "Dummy_WeaponCharacter.generated.h"

class USpringArmComponent;	// �������� ������Ʈ ���漱��
class UCameraComponent;	// ī�޶� ������Ʈ ���漱��
struct FInputActionValue;	// ��ǲ�׼� ���漱��

UCLASS()
class GIVEMETHEMONEY_API ADummy_WeaponCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADummy_WeaponCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dummy_WeaponCharcter")
	USpringArmComponent* SpringArmComp;	// �������� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dummy_WeaponCharcter")
	UCameraComponent* CameraComp;	// ī�޶� ������Ʈ

protected:
	// �Է� ���ε� �Լ�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Input
public:

	UFUNCTION()
	void Move(const FInputActionValue& value);	// �̵��� ����� �Է� �̺�Ʈ
	UFUNCTION()
	void Look(const FInputActionValue& value);	// �ü� �Է� �̺�Ʈ
	UFUNCTION()
	void JumpEvent(const FInputActionValue& value);	// ���� �̺�Ʈ
	UFUNCTION()
	void Shot(const FInputActionValue& value);	// ���� ���� �̺�Ʈ
	UFUNCTION()
	void Reload(const FInputActionValue& value);	// ������ �̺�Ʈ
	UFUNCTION()
	void ChangeWeaponOne(const FInputActionValue& value);	// 1�� ���� ���� �̺�Ʈ
	UFUNCTION()
	void ChangeWeaponTwo(const FInputActionValue& value);	// 2�� ���� ���� �̺�Ʈ

public:
	UPROPERTY(EditAnywhere, Category = "Dummy_WeaponCharcter")
	float MoveSpeed;	// �̵��ӵ�
};
