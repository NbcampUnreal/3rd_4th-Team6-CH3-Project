#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GM_AICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class GIVEMETHEMONEY_API AGM_AICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGM_AICharacter();

	UPROPERTY(BlueprintAssignable, Category = "AI|Event")
	FOnAttackEndDelegate OnAttackEnd;
	UPROPERTY(EditAnywhere, Category = "AI|Attack")
	UAnimMontage* AttackMontage;

	void Attack();
	UFUNCTION()
	void AttackEnd();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	int CurrentHP;  // ���� ü��
	float Speed;   // �̵� �ӵ�
	int MaxHP; // �ִ�ü��


	void TakeDamage(); // ������ ó��
	void Death(); // ����
	void DropCoin(); // ���� �� ���� ����߸�

protected:
	virtual void BeginPlay() override;


public:

};