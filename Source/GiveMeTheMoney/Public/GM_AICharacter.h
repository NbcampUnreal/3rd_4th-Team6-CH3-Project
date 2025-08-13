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

	int CurrentHP;  // 현재 체력
	float Speed;   // 이동 속도
	int MaxHP; // 최대체력


	void TakeDamage(); // 데미지 처리
	void Death(); // 죽음
	void DropCoin(); // 죽은 후 코인 떨어뜨림

protected:
	virtual void BeginPlay() override;


public:

};