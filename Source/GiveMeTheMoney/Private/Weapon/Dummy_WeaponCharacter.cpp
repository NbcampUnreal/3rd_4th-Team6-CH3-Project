// �ÿ��� ���� ĳ����
#include "Weapon/Dummy_WeaponCharacter.h"
#include "Weapon/GM_Dummy_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"	// ������ �� ������Ʈ ���
#include "GameFramework/CharacterMovementComponent.h"	// ĳ���� ������ ������Ʈ ���

// Sets default values
ADummy_WeaponCharacter::ADummy_WeaponCharacter()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);	// Root ������Ʈ�� ����
	SpringArmComp->TargetArmLength = 350.0f;	// ������ ���� Ÿ�� �Ÿ�
	SpringArmComp->bUsePawnControlRotation = true;	// ĳ������ ȸ���� ���

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);	// ������ �� ������Ʈ�� Attach
	CameraComp->bUsePawnControlRotation = false;	// ĳ������ ȸ���� ������� ����

	MoveSpeed = 300.0f;	// ĳ������ �̵� �ӵ�
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;	// ĳ������ �ִ� �̵� �ӵ� ����
}


// �Է� ���ε�
void ADummy_WeaponCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EnhacnedInput �� �ޱ�
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))	
	{
		if (AGM_Dummy_PlayerController* PlayerController = Cast<AGM_Dummy_PlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)	//Move �Է� ���ε�
			{
				EnhancedInput->BindAction(	
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ADummy_WeaponCharacter::Move
				);
			}

			if (PlayerController->LookAction)	//Look �Է� ���ε�
			{
				EnhancedInput->BindAction(	
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ADummy_WeaponCharacter::Look
				);
			}

			if (PlayerController->JumpAction)	//Jump �Է� ���ε�
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Started,	// Jump �ѹ� �Է�
					this,
					&ADummy_WeaponCharacter::JumpEvent
				);
			}

			if (PlayerController->ShotAction)	//Shot �Է� ���ε�
			{
				EnhancedInput->BindAction(
					PlayerController->ShotAction,
					ETriggerEvent::Started,	// Shot �ѹ� �Է�
					this,
					&ADummy_WeaponCharacter::Shot
				);
			}

			if (PlayerController->ReloadAction)	//Reload �Է� ���ε�
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Started,	// Reload �ѹ� �Է�
					this,
					&ADummy_WeaponCharacter::Reload
				);
			}
			
			if (PlayerController->ChangeWeaponOneAction)	//1�� ���� ���� �Է� ���ε�
			{
				EnhancedInput->BindAction(
					PlayerController->ChangeWeaponOneAction,
					ETriggerEvent::Started,	// Reload �ѹ� �Է�
					this,
					&ADummy_WeaponCharacter::ChangeWeaponOne
				);
			}

			if (PlayerController->ChangeWeaponTwoAction)	//2�� ���� ���� �Է� ���ε�
			{
				EnhancedInput->BindAction(
					PlayerController->ChangeWeaponTwoAction,
					ETriggerEvent::Started,	// Reload �ѹ� �Է�
					this,
					&ADummy_WeaponCharacter::ChangeWeaponTwo
				);
			}
		}
	}
}

// �̵� �Է� �̺�Ʈ 
void ADummy_WeaponCharacter::Move(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Move!!!")));
}

// ȸ�� �Է� �̺�Ʈ
void ADummy_WeaponCharacter::Look(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Look!!!")));
}

// ���� �Է� �̺�Ʈ
void ADummy_WeaponCharacter::JumpEvent(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, FString::Printf(TEXT("Jump!!!")));
}

// Shot �Է� �̺�Ʈ
void ADummy_WeaponCharacter::Shot(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Shot!!!")));
}

// Reload �Է� �̺�Ʈ
void ADummy_WeaponCharacter::Reload(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Reload!!!")));
}

// 1�� ���� ���� �Է� �̺�Ʈ
void ADummy_WeaponCharacter::ChangeWeaponOne(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("ChangeWeaponOne!!")));
}

// 2�� ���� ���� �Է� �̺�Ʈ
void ADummy_WeaponCharacter::ChangeWeaponTwo(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("ChangeWeaponTwo!!")));
}