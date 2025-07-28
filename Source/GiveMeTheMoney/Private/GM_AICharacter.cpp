#include "GM_AICharacter.h"
#include "GM_AIController.h"
//#include "GM_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

AGM_AICharacter::AGM_AICharacter()
{
	AIControllerClass = AGM_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	bUseControllerRotationYaw = false;
	Movement->bUseControllerDesiredRotation = false;
	Movement->MaxWalkSpeed = 400.0f;
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

void AGM_AICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("AI character has been spawned."));
}

void AGM_AICharacter::Attack()
{
	if (!AttackMontage) return;
	if (!GetMesh()) return;

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (!AnimInst) return;
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AGM_AICharacter::OnAttackMontageEnded);

	AnimInst->Montage_Play(AttackMontage);
	AnimInst->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	//UE_LOG(LogTemp, Log, TEXT("[%s] Attack Montage Played"), *GetName());


	//auto AnimInstance = Cast<UGM_AnimInstance>(GetMesh()->GetAnimInstance());
	//if (nullptr == AnimInstance) return;

	//AnimInstance->PlayAttackMontage();
}

void AGM_AICharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != AttackMontage) return;

	AttackEnd();
}

void AGM_AICharacter::AttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Attack End"), *GetName());
	OnAttackEnd.Broadcast();
}
