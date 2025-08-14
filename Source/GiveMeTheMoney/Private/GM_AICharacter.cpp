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

float AGM_AICharacter::GetHealth() const	// 현재 체력을 반환하는 함수
{
	return Health;
}

void AGM_AICharacter::AddHealth(float Amount) {	// 체력을 증가시키는 함수
	if (Amount <= 0.0f) return;	// 증가할 양이 0 이하라면 아무것도 하지 않음

	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);	// 현재 체력을 증가시키고 0과 MaxHealth 사이로 제한
	UE_LOG(LogTemp, Warning, TEXT("[AI] Health increased to: %f"), Health);	// 현재 체력 로그 출력
}

float AGM_AICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	// ActualDamage는 부모 클래스의 TakeDamage를 호출하여 기본 데미지 처리 로직을 가져온다.
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	// 부모 클래스의 TakeDamage 호출

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);	// 현재 체력을 감소시키고 0과 MaxHealth 사이로 제한
	UE_LOG(LogTemp, Warning, TEXT("[AI] Health decreased to: %f"), Health);	// 현재 체력 로그 출력

	if (Health <= 0.0f)
	{
		OnDeath();	// 체력이 0 이하가 되면 OnDeath 함수를 호출
	}

	return 0.0f;
}

void AGM_AICharacter::DropCoin()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Dropped coins"), *GetName());
}

void AGM_AICharacter::OnDeath()
{
	// 게임 종료 로직 구현 예정

}