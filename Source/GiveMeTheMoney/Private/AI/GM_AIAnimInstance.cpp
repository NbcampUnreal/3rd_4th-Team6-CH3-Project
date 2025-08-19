#include "AI/GM_AIAnimInstance.h"
#include "AI/GM_AICharacter.h"
#include "Kismet/GameplayStatics.h"

UGM_AIAnimInstance::UGM_AIAnimInstance()
{
	CurrentPawnSpeed = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/AI/Attack1.Attack1'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;

		UE_LOG(LogTemp, Warning, TEXT("Montage Succeeded"));
	}
}

void UGM_AIAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}
}

void UGM_AIAnimInstance::AnimNotify_OnAttackEnd()
{
	if (AGM_AICharacter* AICharacter = Cast<AGM_AICharacter>(TryGetPawnOwner()))
	{
		AICharacter->AttackEnd();
	}

}
