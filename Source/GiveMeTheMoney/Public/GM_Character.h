// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GM_Character.generated.h"

class USpringArmComponent;// �̸� ����
class UCameraComponent;// ������ ��, ī�޶� ������Ʈ Ŭ������ ����� �� �ֵ��� ����
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;// Move �Է� ���� �ޱ� ���� ����ü ����


UCLASS()
class GIVEMETHEMONEY_API AGM_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGM_Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")// ���� �Ӽ��� ���� ������
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")// ���� �Ӽ��� ���� ������
		UCameraComponent* CameraComp;

	UFUNCTION(BlueprintPure, Category = "Health")// �������Ʈ���� �б� �������� ��� ����
		float GetHealth() const;// ���� ü���� ��ȯ�ϴ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Health")// �������Ʈ���� �б� �������� ��� ����
		void AddHealth(float Amount);// ü���� ȸ����Ű�� �Լ�

	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void StartCrouch(); // ��ũ���� ����
	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void StopCrouch();  // ��ũ���� ����

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")// �����Ϳ��� ���� ����
		float MaxHealth;// �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")// �����Ϳ��� ���� ����
		float Health;// ���� ü��

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;// �Է� �̺�Ʈ�� ó���� �Լ�

	virtual float TakeDamage(
		float DamageAmount,// ������ ��
		struct FDamageEvent const& DamageEvent,    // ������ �̺�Ʈ ����ü ��ų �ý����̳� �ٸ� �ý��ۿ��� ������ �̺�Ʈ�� ó���� �� ���
		AController* EventInstigator,// �������� ���� ��ü (�÷��̾�, AI ��)
		AActor* DamageCauser// �������� ���� ���� (����, ���� ��)
	) override;// �������� �޾��� �� ȣ��Ǵ� �Լ� (�⺻ ������ 0�� ��ȯ, �ڽ� Ŭ�������� �������̵� ����

	UFUNCTION()
	//FInputActionValue& �����ڸ� ���� ����ü�� �����͸� �״�� ������(����� �޸� ������ ����ȭ ����) Const�� ���� ��ȣ
	void Move(const FInputActionValue& value);// IA_Move �� �Է� ��(Vector2D�� X,Y�࿡ ����)�� ����

	UFUNCTION()
	// Jump �� ���� On/Off ���۵��� Start�� Stop ������ ����� ���� ���ش�.
	void StartJump(const FInputActionValue& value);//Jump �� ������ ��/ ������� ���� ����
	UFUNCTION()
	void StopJump(const FInputActionValue& value);//Jump ������� ��
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);// ���� ����
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);// ���� ����
	UFUNCTION()
	void Look(const FInputActionValue& value);// ���콺 �Է�(�ü� ó��)

	void Fire(const FInputActionValue& value);// �ѱ� �߻� �α�
	void FireComplete(const FInputActionValue& value);
	void Crouch(const FInputActionValue& value);

	// ��ũ���� ���¸� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	bool bIsCrouching = false; // ���� ��ũ�� ��������

	void OnDeath();// ĳ���Ͱ� �׾��� �� ȣ��Ǵ� �Լ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; // �Ϲ����� �ȱ� �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed; // ���� �ӵ�

	// ���� ��ü �Լ�
	UFUNCTION()
	void EquipWeapon1(const FInputActionValue& value); // 1�� ���� ����
	UFUNCTION()
	void EquipWeapon2(const FInputActionValue& value); // 2�� ���� ����

private:
	//float NormalSpeed;// ������ �Ϲ� �ӵ�
	float SprintSpeedMultiplier;// Normal���� ��踦 �������� �����ϴ� ����
	//float SprintSpeed;// ���� �ӵ� (Normal * Multiplier)
};