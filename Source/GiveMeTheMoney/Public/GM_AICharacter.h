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

	UFUNCTION(BlueprintPure, Category = "Health")// �������Ʈ���� �б� �������� ��� ����
	float GetHealth() const;// ���� ü���� ��ȯ�ϴ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Health")// �������Ʈ���� �б� �������� ��� ����
	void AddHealth(float Amount);// ü���� ȸ����Ű�� �Լ�

	void DropCoin(); // ���� �� ���� ����߸�

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")// �����Ϳ��� ���� ����
	float MaxHealth;// �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")// �����Ϳ��� ���� ����
	float Health;// ���� ü��

	virtual float TakeDamage(
		float DamageAmount,// ������ ��
		struct FDamageEvent const& DamageEvent,    // ������ �̺�Ʈ ����ü ��ų �ý����̳� �ٸ� �ý��ۿ��� ������ �̺�Ʈ�� ó���� �� ���
		AController* EventInstigator,// �������� ���� ��ü (�÷��̾�, AI ��)
		AActor* DamageCauser// �������� ���� ���� (����, ���� ��)
	) override;// �������� �޾��� �� ȣ��Ǵ� �Լ� (�⺻ ������ 0�� ��ȯ, �ڽ� Ŭ�������� �������̵� ����

	void OnDeath();// ĳ���Ͱ� �׾��� �� ȣ��Ǵ� �Լ�

public:

};