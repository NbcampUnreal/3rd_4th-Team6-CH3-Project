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
		FVector Velocity = player->GetVelocity(); // �ӵ� , ������ ���� �ӵ�, ������ ���� �ӵ�

		FVector RightVector = player->GetActorRightVector();
		HDirectionSpeed = FVector::DotProduct(RightVector, Velocity);
		// ���� ���� - ��� , �ٸ� ���� - ����, ���� - 0  ,  ������ �������� �󸶳� ������ �����̴��� ����

		FVector ForwardVector = player->GetActorForwardVector();
		VDirectionSpeed = FVector::DotProduct(ForwardVector, Velocity);
		// ���� ���� - ��� , �ٸ� ���� - ����, ���� - 0  ,  �� �������� �󸶳� ������ �����̴��� ����

		auto Movement = player->GetCharacterMovement();
		IsFalling = Movement->IsFalling();
	}
}
