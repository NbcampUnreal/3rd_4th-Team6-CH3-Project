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

	UFUNCTION(BlueprintPure, Category = "Health")// 블루프린트에서 읽기 전용으로 사용 가능
	float GetHealth() const;// 현재 체력을 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Health")// 블루프린트에서 읽기 전용으로 사용 가능
	void AddHealth(float Amount);// 체력을 회복시키는 함수

	void DropCoin(); // 죽은 후 코인 떨어뜨림

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")// 에디터에서 조정 가능
	float MaxHealth;// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")// 에디터에서 조정 가능
	float Health;// 현재 체력

	virtual float TakeDamage(
		float DamageAmount,// 데미지 양
		struct FDamageEvent const& DamageEvent,    // 데미지 이벤트 구조체 스킬 시스템이나 다른 시스템에서 데미지 이벤트를 처리할 때 사용
		AController* EventInstigator,// 데미지를 입힌 주체 (플레이어, AI 등)
		AActor* DamageCauser// 데미지를 입힌 원인 (무기, 폭발 등)
	) override;// 데미지를 받았을 때 호출되는 함수 (기본 구현은 0을 반환, 자식 클래스에서 오버라이드 가능

	void OnDeath();// 캐릭터가 죽었을 때 호출되는 함수

public:

};