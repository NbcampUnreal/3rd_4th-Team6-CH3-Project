#include "GM_AICharacter.h"
#include "GM_AIController.h"
#include "GM_AIAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

AGM_AICharacter::AGM_AICharacter()
{
	AIControllerClass = AGM_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	bUseControllerRotationYaw = false;
	Movement->bUseControllerDesiredRotation = false;
	Movement->MaxWalkSpeed = 300.0f;
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

	UE_LOG(LogTemp, Log, TEXT("[%s] Attack Montage Played"), *GetName());


	auto AnimInstance = Cast<UGM_AIAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance) return;

	AnimInstance->PlayAttackMontage();
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

float AGM_AICharacter::GetHealth() const	// ���� ü���� ��ȯ�ϴ� �Լ�
{
	return Health;
}

void AGM_AICharacter::AddHealth(float Amount) {	// ü���� ������Ű�� �Լ�
	if (Amount <= 0.0f) return;	// ������ ���� 0 ���϶�� �ƹ��͵� ���� ����

	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);	// ���� ü���� ������Ű�� 0�� MaxHealth ���̷� ����
	UE_LOG(LogTemp, Warning, TEXT("[AI] Health increased to: %f"), Health);	// ���� ü�� �α� ���
}

float AGM_AICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	// ActualDamage�� �θ� Ŭ������ TakeDamage�� ȣ���Ͽ� �⺻ ������ ó�� ������ �����´�.
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	// �θ� Ŭ������ TakeDamage ȣ��

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);	// ���� ü���� ���ҽ�Ű�� 0�� MaxHealth ���̷� ����
	UE_LOG(LogTemp, Warning, TEXT("[AI] Health decreased to: %f"), Health);	// ���� ü�� �α� ���

	if (Health <= 0.0f)
	{
		OnDeath();	// ü���� 0 ���ϰ� �Ǹ� OnDeath �Լ��� ȣ��
	}

	return 0.0f;
}

void AGM_AICharacter::DropCoin()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Dropped coins"), *GetName());
}

void AGM_AICharacter::OnDeath()
{
	// ���� ���� ���� ���� ����

}