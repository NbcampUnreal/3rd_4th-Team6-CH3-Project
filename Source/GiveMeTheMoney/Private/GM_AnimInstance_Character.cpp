// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_AnimInstance_Character.h"

#include "GM_Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGM_AnimInstance_Character::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* ownerPawn = TryGetPawnOwner();
	auto player = Cast<AGM_Character>(ownerPawn);
	if (player)
	{
		FVector Velocity = player->GetVelocity(); // 속도 , 앞으로 가는 속도, 옆으로 가는 속도

		FVector RightVector = player->GetActorRightVector();
		HDirectionSpeed = FVector::DotProduct(RightVector, Velocity);
		// 같은 방향 - 양수 , 다른 방향 - 음수, 수직 - 0  ,  오른쪽 방향으로 얼마나 빠르게 움직이는지 측정

		FVector ForwardVector = player->GetActorForwardVector();
		VDirectionSpeed = FVector::DotProduct(ForwardVector, Velocity);
		// 같은 방향 - 양수 , 다른 방향 - 음수, 수직 - 0  ,  앞 방향으로 얼마나 빠르게 움직이는지 측정

		auto Movement = player->GetCharacterMovement();
		IsFalling = Movement->IsFalling();
	}
}
