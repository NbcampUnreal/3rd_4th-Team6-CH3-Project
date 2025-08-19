#include "AI/GM_AICharacter.h"
#include "AI/GM_AIController.h"
#include "AI/GM_AIAnimInstance.h"
#include "GameModes/GM_GameModeBase.h"	// ���Ӹ�� ���
#include "../GM_Coin.h"	// ���� ���� ��� 
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"

AGM_AICharacter::AGM_AICharacter()
{
	AIControllerClass = AGM_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	CapsuleComp = GetCapsuleComponent();	// AI ĸ�� ������Ʈ ������
	AttackBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBoxCollision"));	// �ٰŸ� ���ݹ��� 
	AttackBoxCollision->SetupAttachment(CapsuleComp);	
	AttackBoxCollision->SetRelativeLocation(FVector(50, 0, 0));
	AttackBoxCollision->SetBoxExtent(FVector(100, 100, 100));
	AttackBoxCollision->bHiddenInGame = true;	// ���� �� ǥ�� ����

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

	//UE_LOG(LogTemp, Warning, TEXT("AI character has been spawned."));
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
	// ������ �����Լ� �߰�����
	OnHitAttack();
	AnimInst->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	//UE_LOG(LogTemp, Log, TEXT("[%s] Attack Montage Played"), *GetName());


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
	//UE_LOG(LogTemp, Warning, TEXT("[%s] Attack End"), *GetName());
	OnAttackEnd.Broadcast();
}

float AGM_AICharacter::GetHealth() const	// ���� ü���� ��ȯ�ϴ� �Լ�
{
	return Health;
}

void AGM_AICharacter::AddHealth(float Amount) {	// ü���� ������Ű�� �Լ�
	if (Amount <= 0.0f) return;	// ������ ���� 0 ���϶�� �ƹ��͵� ���� ����

	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);	// ���� ü���� ������Ű�� 0�� MaxHealth ���̷� ����
	//UE_LOG(LogTemp, Warning, TEXT("[AI] Health increased to: %f"), Health);	// ���� ü�� �α� ���
}

float AGM_AICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	// ActualDamage�� �θ� Ŭ������ TakeDamage�� ȣ���Ͽ� �⺻ ������ ó�� ������ �����´�.
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	// �θ� Ŭ������ TakeDamage ȣ��

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);	// ���� ü���� ���ҽ�Ű�� 0�� MaxHealth ���̷� ����
	//UE_LOG(LogTemp, Warning, TEXT("[AI] Health decreased to: %f"), Health);	// ���� ü�� �α� ���

	if (Health <= 0.0f)
	{
		OnDeath();	// ü���� 0 ���ϰ� �Ǹ� OnDeath �Լ��� ȣ��
	}

	return 0.0f;
}

void AGM_AICharacter::OnHitAttack()
{
	//UE_LOG(LogTemp, Error, TEXT("[AI] Attack!"));	// ���� ü�� �α� ���
	TArray<AActor*> OverlappingActors;
	if (AttackBoxCollision)
	{
		AttackBoxCollision->GetOverlappingActors(OverlappingActors);	// ���������� ���� �迭�� ����
		for (AActor* OtherActor : OverlappingActors)
		{
			if (OtherActor && OtherActor->ActorHasTag("Player"))
			{
				//UE_LOG(LogTemp, Error, TEXT("[AI] Attack! Player"));	// ���� ü�� �α� ���
				UGameplayStatics::ApplyDamage(
					OtherActor,
					10,	// ������ ��
					nullptr,
					this,
					UDamageType::StaticClass()
				);
			}
		}
	}
}

void AGM_AICharacter::DropCoin()
{
	//UE_LOG(LogTemp, Warning, TEXT("[%s] Dropped coins"), *GetName());
	// ���� ���� ����
	if (CoinClass) {
		AGM_Coin* Coin =
			GetWorld()->SpawnActor<AGM_Coin>(CoinClass, GetActorLocation(), GetActorRotation());
	}
}

void AGM_AICharacter::OnDeath()
{
	// ���� �� ���� ��� �� AI ĳ���� ����
	// 
	//���� ���� ��� ��������
	AGM_GameModeBase* MyGameMode = Cast<AGM_GameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsValid(MyGameMode))
	{
		MyGameMode->OnEnemyKilled();	// GameMode���� Kill Count ����
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Found MyGameMode!"));
	}
	DropCoin();	// ���� ���
	// AGM_GameModeBase::OnEnemyKilled()
	Destroy();
}