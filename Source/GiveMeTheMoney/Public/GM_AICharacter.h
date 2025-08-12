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

protected:
	virtual void BeginPlay() override;


public:

};