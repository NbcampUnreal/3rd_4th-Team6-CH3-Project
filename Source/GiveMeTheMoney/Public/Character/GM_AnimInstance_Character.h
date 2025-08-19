// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GM_AnimInstance_Character.generated.h"

/**
 * 
 */
UCLASS()
class GIVEMETHEMONEY_API UGM_AnimInstance_Character : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerMoveState")
	float HDirectionSpeed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerMoveState")
	float VDirectionSpeed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerMoveState")
	bool IsFalling = false;
};
