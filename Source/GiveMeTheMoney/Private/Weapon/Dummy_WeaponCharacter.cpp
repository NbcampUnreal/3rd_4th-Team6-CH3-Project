// �ÿ��� ���� ĳ����

#include "Weapon/Dummy_WeaponCharacter.h"
#include "Weapon/GM_Dummy_PlayerController.h"
#include "Weapon/GM_Weapon_Rifle.h"
#include "Weapon/GM_Weapon_Shotgun.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"	// ������ �� ������Ʈ ���
#include "GameFramework/CharacterMovementComponent.h"	// ĳ���� ������ ������Ʈ ���

// Sets default values
ADummy_WeaponCharacter::ADummy_WeaponCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);	//��Ʈ ������Ʈ ����ġ
	SpringArmComp->TargetArmLength = 300.0f;		// ������ ���� ����
	SpringArmComp->bUsePawnControlRotation = true;	// ��Ʈ�ѷ��� ������ �� ȸ�� ���

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);	//ī�޶� ������Ʈ�� �������� �� ���� ����
	CameraComp->bUsePawnControlRotation = false;	// ��Ʈ�ѷ��� ī�޶� ȸ�� ���ϰ� ����

	NormalSpeed = 200.0f;	// ���� �ӵ� 
	SprintSpeedMultiplier = 3.0f;	// ������ ũ��
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	MaxHealth = 100.0f;	// �ִ� ü�� ����
	Health = MaxHealth;	// ���� ü�� ����

}

// �Է� ���� ���� ��� ó�� �� �� �����ϴ� ��
void ADummy_WeaponCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//���� ĳ������ ��Ʈ�ѷ��� ������ �� SpartaPlayerController�� ĳ����
		if (AGM_Dummy_PlayerController* PlayerController = Cast<AGM_Dummy_PlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)	// W,A,S,D ������ �Է� ó��
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,	// Ű�� �Է� �Ǿ��� ��
					this,						// ȣ��� ��ü
					&ADummy_WeaponCharacter::Move		// ��ü�� Move �Լ��� ����
				);
			}

			if (PlayerController->LookAction)	// ī�޶� �ü� �Է� ó��
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,	// Ű�� �Է� �Ǿ��� ��
					this,						// ȣ��� ��ü
					&ADummy_WeaponCharacter::Look		// ��ü�� Look �Լ��� ����
				);
			}

			if (PlayerController->JumpAction)	//���� �Է� ó��
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,	// Ű�� �Է� �Ǿ��� ��
					this,						// ȣ��� ��ü
					&ADummy_WeaponCharacter::StartJump		// ���� Ű�� ������ �� �Է� ó��
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,	// Ű�� �� ���� StopJump ȣ��
					this,						// ȣ��� ��ü
					&ADummy_WeaponCharacter::StopJump		// ���� Ű�� ������ �� �Է� ó��
				);
			}

			if (PlayerController->SprintAction)		// ���� �Է� ó��
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,	// Ű�� �Է� �Ǿ��� ��
					this,						// ȣ��� ��ü
					&ADummy_WeaponCharacter::StartSprint		// ���� Ű�� ������ �� �Է� ó��
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,	// Ű�� �� ���� StopSprint ȣ��
					this,						// ȣ��� ��ü
					&ADummy_WeaponCharacter::StopSprint	// ���� Ű�� ������ �� �Է� ó��
				);
			}
			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Started, // Triggered �̺�Ʈ�� ���
					this,
					&ADummy_WeaponCharacter::Crouch
				);
			}
			if (PlayerController->FireAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Started, // Ű�� ������ ����
					this,
					&ADummy_WeaponCharacter::Fire
				);
			}
			if (PlayerController->EquipWeapon1Action)
			{
				EnhancedInput->BindAction(
					PlayerController->EquipWeapon1Action,
					ETriggerEvent::Started, // Ű�� ������ ��
					this,
					&ADummy_WeaponCharacter::EquipWeapon1
				);
			}

			if (PlayerController->EquipWeapon2Action)
			{
				EnhancedInput->BindAction(
					PlayerController->EquipWeapon2Action,
					ETriggerEvent::Started, // Ű�� ������ ��
					this,
					&ADummy_WeaponCharacter::EquipWeapon2
				);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Started, // Ű�� ������ ��
					this,
					&ADummy_WeaponCharacter::Reload
				);
			}
		}
	}
}

void ADummy_WeaponCharacter::Move(const FInputActionValue & value)	// value : 2D vector ������ ����
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

void ADummy_WeaponCharacter::Look(const FInputActionValue & value)	// �ü� ó���� ���� ���콺 ��ǥ ������ 2D Vector�� ���� �޾ƿ�
{
	FVector2D LookInput = value.Get<FVector2D>();	// 2D Vecotr ���� ����

	AddControllerYawInput(LookInput.X);		// ��, �� �Է�
	AddControllerPitchInput(-LookInput.Y);	// ��, �� �Է�
}

void ADummy_WeaponCharacter::StartJump(const FInputActionValue & value) // value : Digit ������ true/false ���� ������ ����
{
	if (value.Get<bool>())	//Jump�Է��� True�� �� StartJump �Լ� ����
	{
		Jump();		// �𸮾� �⺻ ���� ĳ���� ���� ���
	}
}

void ADummy_WeaponCharacter::StopJump(const FInputActionValue & value)
{
	if (!value.Get<bool>())	//Jump�Է��� False�� �� StopJump �Լ� ����
	{
		StopJumping();		// �𸮾� �⺻ ���� ĳ���� ���� ���ߴ� ���
	}
}

void ADummy_WeaponCharacter::StartSprint(const FInputActionValue & value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;		// ���� Ű �Է½� ��� Sprint�ӵ��� ��ȯ
	}
}

void ADummy_WeaponCharacter::StopSprint(const FInputActionValue & value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;		// ���� Ű ������ �� ��� Normal�ӵ��� ��ȯ
	}
}

void ADummy_WeaponCharacter::Crouch(const FInputActionValue & value)
{
	// �Է� ���� �������, �Լ��� ȣ��Ǹ� �ٷ� ��� ���� ����
	if (bIsCrouching)
	{
		StopCrouch();
	}
	else
	{
		StartCrouch();
	}
}

void ADummy_WeaponCharacter::StartCrouch()
{
	if (CanCrouch()) // �𸮾��� �⺻ �Լ��� ����Ͽ� ��ũ�� �� �ִ��� Ȯ��
	{
		ACharacter::Crouch(); // ĳ���͸� ��ũ���� ��
		bIsCrouching = true;  // ���� ������ ������ true�� ����
		/*UE_LOG(LogTemp, Warning, TEXT("Character is crouching."));*/
	}
}

void ADummy_WeaponCharacter::StopCrouch()
{
	ACharacter::UnCrouch(); // ��ũ���� ����
	bIsCrouching = false;   // ���� ������ ������ false�� ����
	/*UE_LOG(LogTemp, Warning, TEXT("Character stopped crouching."));*/
}

void ADummy_WeaponCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateMuzzleTransform();	// �ѱ� ��ġ ������Ʈ
	// Rife, Shotgun ���� ����
	if (Rifle && Shotgun)
	{
		WeaponInventory.Add(GetWorld()->SpawnActor<AGM_Weapon_Rifle>(Rifle, MuzzleSocketLocation, MuzzleSocketRotation));
		WeaponInventory.Add(GetWorld()->SpawnActor<AGM_Weapon_Shotgun>(Shotgun, MuzzleSocketLocation, MuzzleSocketRotation));

		CurrentWeapon = WeaponInventory[0];	// ���۹���� Rifle
	}
}

//void ADummy_WeaponCharacter::Crouch(const FInputActionValue& value)
//{
//	if (value.Get<bool>()) // ��ũ���� Ű�� ������ ��
//	{
//		if (bIsCrouching)
//		{
//			StopCrouch();
//		}
//		else
//		{
//			StartCrouch();
//		}
//	}
//}
//
//void ADummy_WeaponCharacter::StartCrouch()
//{
//	if (CanCrouch()) // �𸮾��� �⺻ �Լ��� ����Ͽ� ��ũ�� �� �ִ��� Ȯ��
//	{
//		ACharacter::Crouch(); // ĳ���͸� ��ũ���� ��
//		bIsCrouching = true;
//		// �ʿ��ϴٸ�, ���⿡ ��ũ���� �ִϸ��̼� ��� ������ �߰�
//		/*UE_LOG(LogTemp, Warning, TEXT("Character is crouching."));*/
//	}
//}
//
//void ADummy_WeaponCharacter::StopCrouch()
//{
//	ACharacter::UnCrouch(); // ��ũ���� ����
//	bIsCrouching = false;
//	// �ʿ��ϴٸ�, ���⿡ ��ũ���� ���� �ִϸ��̼� ��� ������ �߰�
//	/*UE_LOG(LogTemp, Warning, TEXT("Character stopped crouching."));*/
//}



float ADummy_WeaponCharacter::GetHealth() const	// ���� ü���� ��ȯ�ϴ� �Լ�
{
	return Health;
}

void ADummy_WeaponCharacter::AddHealth(float Amount) {	// ü���� ������Ű�� �Լ�
	if (Amount <= 0.0f) return;	// ������ ���� 0 ���϶�� �ƹ��͵� ���� ����

	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);	// ���� ü���� ������Ű�� 0�� MaxHealth ���̷� ����
	UE_LOG(LogTemp, Warning, TEXT("Health increased to: %f"), Health);	// ���� ü�� �α� ���
}

float ADummy_WeaponCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController * EventInstigator, AActor * DamageCauser)
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

void ADummy_WeaponCharacter::Fire(const FInputActionValue & Value)
{
	if (CurrentWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire!!!")));
		// �Ѿ� ���� ��ġ ������Ʈ �� �߻�
		UpdateMuzzleTransform();
		CurrentWeapon->SetActorLocation(MuzzleSocketLocation);
		CurrentWeapon->SetActorRotation(MuzzleSocketRotation);
		if (int32 CurrentAmmo = CurrentWeapon->GetAmmo() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("You Should Reload"));	//ź�� ������ ������ �ʿ� ���
			return;
		}
		CurrentWeapon->ShootBullet();	// Ŭ���� ������ ���� (����� Shotgun) �߻�
		UE_LOG(LogTemp, Warning, TEXT("%d/%d"), CurrentWeapon->GetAmmo(), CurrentWeapon->MaxAmmo);
		CurrentWeapon->CurrentAmmo -= 1;	// ź�� ����
	}
}

void ADummy_WeaponCharacter::EquipWeapon1(const FInputActionValue & value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Equip 1st Weatpon");
	if (WeaponInventory.IsValidIndex(0))
	{
		CurrentWeapon = WeaponInventory[0];	// 1�� Rifle ����
	}
}

void ADummy_WeaponCharacter::EquipWeapon2(const FInputActionValue & value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Equip 2th Weapon");
	if (WeaponInventory.IsValidIndex(1))
	{
		CurrentWeapon = WeaponInventory[1];	// 2�� Shotgun ����
	}
}

void ADummy_WeaponCharacter::Reload(const FInputActionValue & value)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();	// ���� ���� ������
	}
}

void ADummy_WeaponCharacter::UpdateMuzzleTransform()
{
	MuzzleSocketLocation = GetMesh()->GetSocketLocation("Muzzle");
	MuzzleSocketRotation = GetMesh()->GetSocketRotation("Muzzle");
}


void ADummy_WeaponCharacter::OnDeath()
{
	// ���� ���� ���� ���� ����

}