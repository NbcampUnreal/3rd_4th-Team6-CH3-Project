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
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);	// Root ������Ʈ�� ����
	SpringArmComp->TargetArmLength = 350.0f;	// ������ ���� Ÿ�� �Ÿ�
	SpringArmComp->bUsePawnControlRotation = true;	// ĳ������ ȸ���� ���

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);	// ������ �� ������Ʈ�� Attach
	CameraComp->bUsePawnControlRotation = false;	// ĳ������ ȸ���� ������� ����

	MoveSpeed = 600.0f;	// ĳ������ �̵� �ӵ�
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
					ETriggerEvent::Started,
					this,
					&ADummy_WeaponCharacter::StartJumpEvent
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

void ADummy_WeaponCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Rifle = GetWorld()->SpawnActor<AGM_Weapon_Rifle>(	// Rifle ����
	//	AGM_Weapon_Rifle::StaticClass(),
	//	GetMesh()->GetSocketLocation("Muzzle"),	//�ѱ� ��ġ�� ȸ����
	//	GetMesh()->GetSocketRotation("Muzzle")
	//);
	//Shotgun = GetWorld()->SpawnActor<AGM_Weapon_Shotgun>(	// Shotgun ����
	//	AGM_Weapon_Shotgun::StaticClass(),
	//	GetMesh()->GetSocketLocation("Muzzle"),	//�ѱ� ��ġ�� ȸ����
	//	GetMesh()->GetSocketRotation("Muzzle")
	//);
	
	UpdateMuzzleTransform();
	// Rifle �� Shotgun Ŭ������ �����Ѵٸ� ����
	if (Rifle && Shotgun) 
	{
		WeaponInventory.Add(GetWorld()->SpawnActor<AGM_Weapon_Rifle>(Rifle, MuzzleSocketLocation, MuzzleSocketRotation));
		WeaponInventory.Add(GetWorld()->SpawnActor<AGM_Weapon_Shotgun>(Shotgun, MuzzleSocketLocation, MuzzleSocketRotation));
	
		CurrentWeapon = WeaponInventory[1];	// ���۹���� ����
	}
}

// �̵� �Է� �̺�Ʈ 
void ADummy_WeaponCharacter::Move(const FInputActionValue& value)
{
	if(!Controller) return;	// ĳ���Ͱ� ������ Return
	
	const FVector2D MoveInput = value.Get<FVector2D>();	// �̵� �Է°��� FVector2D�� ���� (W,S) : X , (A,D) : Y
	
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);	// Z��(Yaw) ȸ���� ����

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	// X��(����) ���� ����
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);	// Y��(����) ���� ����
	

	if (!FMath::IsNearlyZero(MoveInput.X))	// W,S ���� �̵�
	{
		AddMovementInput(ForwardDirection, MoveInput.X);
	}
	
	if (!FMath::IsNearlyZero(MoveInput.Y))	// A,D  ��,������ �̵�
	{
		AddMovementInput(RightDirection, MoveInput.Y);
	}
}

// ȸ�� �Է� �̺�Ʈ
void ADummy_WeaponCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);	// Yaw ȸ��
	AddControllerPitchInput(LookInput.Y);	// Pitch ȸ��
}

// ����  �̺�Ʈ
void ADummy_WeaponCharacter::StartJumpEvent(const FInputActionValue& value)
{
	Jump();
}

// Shot �Է� �̺�Ʈ
void ADummy_WeaponCharacter::Shot(const FInputActionValue& value)
{
	if (CurrentWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Shot!!!")));
		// �Ѿ� ���� ��ġ ������Ʈ �� �߻�
		UpdateMuzzleTransform();
		CurrentWeapon->SetActorLocation(MuzzleSocketLocation);
		CurrentWeapon->SetActorRotation(MuzzleSocketRotation);
		CurrentWeapon->ShootBullet();	// Ŭ���� ������ ���� (����� Shotgun) �߻�
	}
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
	SwitchWeapon(0);	// Rifle ���� ����
}

// 2�� ���� ���� �Է� �̺�Ʈ
void ADummy_WeaponCharacter::ChangeWeaponTwo(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("ChangeWeaponTwo!!")));
	SwitchWeapon(1);	// Shotgun ���� ����
}

// ���� ����
void ADummy_WeaponCharacter::SwitchWeapon(int32 WeaponIndex)
{
	if (WeaponInventory.IsValidIndex(WeaponIndex))
	{
		CurrentWeapon = WeaponInventory[WeaponIndex];	// ���� ����
		if (CurrentWeapon)
		{
			CurrentWeapon->Activate();	// ���⿡ �´� Activate ����
		}
	}
}

// �ѱ� ��ġ ������Ʈ
void ADummy_WeaponCharacter::UpdateMuzzleTransform()
{
	MuzzleSocketLocation = GetMesh()->GetSocketLocation("Muzzle");
	MuzzleSocketRotation = GetMesh()->GetSocketRotation("Muzzle");
}
