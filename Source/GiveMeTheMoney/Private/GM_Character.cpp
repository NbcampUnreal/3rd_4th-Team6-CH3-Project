// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Character.h"
#include "GM_Controller_Character.h"
#include "EnhancedInputComponent.h"		// EnhancedInput ���ε��� ����ϱ� ���� �������
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"	// ĳ���� �����Ʈ ������Ʈ �������


AGM_Character::AGM_Character()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);	//��Ʈ ������Ʈ ����ġ
	SpringArmComp->TargetArmLength = 300.0f;		// ������ ���� ����
	SpringArmComp->bUsePawnControlRotation = true;	// ��Ʈ�ѷ��� ������ �� ȸ�� ���

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);	//ī�޶� ������Ʈ�� �������� �� ���� ����
	CameraComp->bUsePawnControlRotation = false;	// ��Ʈ�ѷ��� ī�޶� ȸ�� ���ϰ� ����

	NormalSpeed = 600.0f;	// ���� �ӵ� 
	SprintSpeedMultiplier = 2.0f;	// ������ ũ��
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	MaxHealth = 100.0f;	// �ִ� ü�� ����
	Health = MaxHealth;	// ���� ü�� ����

}


void AGM_Character::Move(const FInputActionValue& value)	// value : 2D vector ������ ����
{
	if (!Controller)	return;	//ĳ���Ͱ� ��ȿ���� Ȯ�� (�������� �ʴٸ� return)

	const FVector2D MoveInput = value.Get<FVector2D>();		//value ���� 2DVector�� �Է� �޾� MoveInput�� ����

	if (!FMath::IsNearlyZero(MoveInput.X))	// �Է� ���� 0�� �ƴ϶��
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);	//�ش� ������ �������� X��ŭ �̵�
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);	// �ش� ������ ���������� Y��ŭ �̵�
	}
}

void AGM_Character::Look(const FInputActionValue& value)	// �ü� ó���� ���� ���콺 ��ǥ ������ 2D Vector�� ���� �޾ƿ�
{
	FVector2D LookInput = value.Get<FVector2D>();	// 2D Vecotr ���� ����

	AddControllerYawInput(LookInput.X);		// ��, �� �Է�
	AddControllerPitchInput(-LookInput.Y);	// ��, �� �Է�
}

void AGM_Character::StartJump(const FInputActionValue& value) // value : Digit ������ true/false ���� ������ ����
{
	if (value.Get<bool>())	//Jump�Է��� True�� �� StartJump �Լ� ����
	{
		Jump();		// �𸮾� �⺻ ���� ĳ���� ���� ���
	}
}

void AGM_Character::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())	//Jump�Է��� False�� �� StopJump �Լ� ����
	{
		StopJumping();		// �𸮾� �⺻ ���� ĳ���� ���� ���ߴ� ���
	}
}

void AGM_Character::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;		// ���� Ű �Է½� ��� Sprint�ӵ��� ��ȯ
	}
}

void AGM_Character::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{

		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;		// ���� Ű ������ �� ��� Normal�ӵ��� ��ȯ
	}
}

// �Է� ���� ���� ��� ó�� �� �� �����ϴ� ��
void AGM_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//���� ĳ������ ��Ʈ�ѷ��� ������ �� SpartaPlayerController�� ĳ����
		if (AGM_Controller_Character* PlayerController = Cast<AGM_Controller_Character>(GetController()))
		{
			if (PlayerController->MoveAction)	// W,A,S,D ������ �Է� ó��
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,	// Ű�� �Է� �Ǿ��� ��
					this,						// ȣ��� ��ü
					&AGM_Character::Move		// ��ü�� Move �Լ��� ����
				);
			}

			if (PlayerController->LookAction)	// ī�޶� �ü� �Է� ó��
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,	// Ű�� �Է� �Ǿ��� ��
					this,						// ȣ��� ��ü
					&AGM_Character::Look		// ��ü�� Look �Լ��� ����
				);
			}

			if (PlayerController->JumpAction)	//���� �Է� ó��
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,	// Ű�� �Է� �Ǿ��� ��
					this,						// ȣ��� ��ü
					&AGM_Character::StartJump		// ���� Ű�� ������ �� �Է� ó��
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,	// Ű�� �� ���� StopJump ȣ��
					this,						// ȣ��� ��ü
					&AGM_Character::StopJump		// ���� Ű�� ������ �� �Է� ó��
				);
			}

			if (PlayerController->SprintAction)		// ���� �Է� ó��
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,	// Ű�� �Է� �Ǿ��� ��
					this,						// ȣ��� ��ü
					&AGM_Character::StartSprint		// ���� Ű�� ������ �� �Է� ó��
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,	// Ű�� �� ���� StopSprint ȣ��
					this,						// ȣ��� ��ü
					&AGM_Character::StopSprint	// ���� Ű�� ������ �� �Է� ó��
				);
			}
		}
	}
}

float AGM_Character::GetHealth() const	// ���� ü���� ��ȯ�ϴ� �Լ�
{
	return Health;
}

void AGM_Character::AddHealth(float Amount) {	// ü���� ������Ű�� �Լ�
	if (Amount <= 0.0f) return;	// ������ ���� 0 ���϶�� �ƹ��͵� ���� ����

	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);	// ���� ü���� ������Ű�� 0�� MaxHealth ���̷� ����
	UE_LOG(LogTemp, Warning, TEXT("Health increased to: %f"), Health);	// ���� ü�� �α� ���
}

float AGM_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	// ActualDamage�� �θ� Ŭ������ TakeDamage�� ȣ���Ͽ� �⺻ ������ ó�� ������ �����´�.
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	// �θ� Ŭ������ TakeDamage ȣ��

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);	// ���� ü���� ���ҽ�Ű�� 0�� MaxHealth ���̷� ����
	UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);	// ���� ü�� �α� ���

	if (Health <= 0.0f)
	{
		OnDeath();	// ü���� 0 ���ϰ� �Ǹ� OnDeath �Լ��� ȣ��
	}

	return 0.0f;
}

void AGM_Character::OnDeath()
{
	// ���� ���� ���� ���� ����

}