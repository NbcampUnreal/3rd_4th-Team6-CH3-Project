#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GM_AIAnimInstance.generated.h"

UCLASS()
class GIVEMETHEMONEY_API UGM_AIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UGM_AIAnimInstance();
	//virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	UFUNCTION()
	void AnimNotify_OnAttackEnd();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UAnimMontage* AttackMontage;

};
