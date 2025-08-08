// 시연용 더미 캐릭터

#include "Weapon/Dummy_WeaponCharacter.h"
#include "Weapon/GM_Dummy_PlayerController.h"
#include "Weapon/GM_Weapon_Rifle.h"
#include "Weapon/GM_Weapon_Shotgun.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"	// 스프링 암 컴포넌트 헤더
#include "GameFramework/CharacterMovementComponent.h"	// 캐릭터 움직임 컴포넌트 헤더

// Sets default values
ADummy_WeaponCharacter::ADummy_WeaponCharacter()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);	// Root 컴포넌트로 설정
	SpringArmComp->TargetArmLength = 350.0f;	// 스프링 암의 타겟 거리
	SpringArmComp->bUsePawnControlRotation = true;	// 캐릭터의 회전을 사용

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);	// 스프링 암 컴포넌트로 Attach
	CameraComp->bUsePawnControlRotation = false;	// 캐릭터의 회전을 사용하지 않음

	MoveSpeed = 600.0f;	// 캐릭터의 이동 속도
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;	// 캐릭터의 최대 이동 속도 설정
}


// 입력 바인딩
void ADummy_WeaponCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EnhacnedInput 값 받기
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))	
	{
		if (AGM_Dummy_PlayerController* PlayerController = Cast<AGM_Dummy_PlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)	//Move 입력 바인딩
			{
				EnhancedInput->BindAction(	
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ADummy_WeaponCharacter::Move
				);
			}

			if (PlayerController->LookAction)	//Look 입력 바인딩
			{
				EnhancedInput->BindAction(	
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ADummy_WeaponCharacter::Look
				);
			}

			if (PlayerController->JumpAction)	//Jump 입력 바인딩
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Started,
					this,
					&ADummy_WeaponCharacter::StartJumpEvent
				);
			}

			if (PlayerController->ShotAction)	//Shot 입력 바인딩
			{
				EnhancedInput->BindAction(
					PlayerController->ShotAction,
					ETriggerEvent::Started,	// Shot 한번 입력
					this,
					&ADummy_WeaponCharacter::Shot
				);
			}

			if (PlayerController->ReloadAction)	//Reload 입력 바인딩
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Started,	// Reload 한번 입력
					this,
					&ADummy_WeaponCharacter::Reload
				);
			}
			
			if (PlayerController->ChangeWeaponOneAction)	//1번 무기 변경 입력 바인딩
			{
				EnhancedInput->BindAction(
					PlayerController->ChangeWeaponOneAction,
					ETriggerEvent::Started,	// Reload 한번 입력
					this,
					&ADummy_WeaponCharacter::ChangeWeaponOne
				);
			}

			if (PlayerController->ChangeWeaponTwoAction)	//2번 무기 변경 입력 바인딩
			{
				EnhancedInput->BindAction(
					PlayerController->ChangeWeaponTwoAction,
					ETriggerEvent::Started,	// Reload 한번 입력
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

	//Rifle = GetWorld()->SpawnActor<AGM_Weapon_Rifle>(	// Rifle 생성
	//	AGM_Weapon_Rifle::StaticClass(),
	//	GetMesh()->GetSocketLocation("Muzzle"),	//총구 위치와 회전값
	//	GetMesh()->GetSocketRotation("Muzzle")
	//);
	//Shotgun = GetWorld()->SpawnActor<AGM_Weapon_Shotgun>(	// Shotgun 생성
	//	AGM_Weapon_Shotgun::StaticClass(),
	//	GetMesh()->GetSocketLocation("Muzzle"),	//총구 위치와 회전값
	//	GetMesh()->GetSocketRotation("Muzzle")
	//);
	
	UpdateMuzzleTransform();
	// Rifle 와 Shotgun 클래스가 존재한다면 스폰
	if (Rifle && Shotgun) 
	{
		WeaponInventory.Add(GetWorld()->SpawnActor<AGM_Weapon_Rifle>(Rifle, MuzzleSocketLocation, MuzzleSocketRotation));
		WeaponInventory.Add(GetWorld()->SpawnActor<AGM_Weapon_Shotgun>(Shotgun, MuzzleSocketLocation, MuzzleSocketRotation));
	
		CurrentWeapon = WeaponInventory[1];	// 시작무기는 샷건
	}
}

// 이동 입력 이벤트 
void ADummy_WeaponCharacter::Move(const FInputActionValue& value)
{
	if(!Controller) return;	// 캐릭터가 없으면 Return
	
	const FVector2D MoveInput = value.Get<FVector2D>();	// 이동 입력값을 FVector2D로 받음 (W,S) : X , (A,D) : Y
	
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);	// Z축(Yaw) 회전값 저장

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	// X축(전방) 방향 벡터
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);	// Y축(우측) 방향 벡터
	

	if (!FMath::IsNearlyZero(MoveInput.X))	// W,S 전방 이동
	{
		AddMovementInput(ForwardDirection, MoveInput.X);
	}
	
	if (!FMath::IsNearlyZero(MoveInput.Y))	// A,D  왼,오른쪽 이동
	{
		AddMovementInput(RightDirection, MoveInput.Y);
	}
}

// 회전 입력 이벤트
void ADummy_WeaponCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);	// Yaw 회전
	AddControllerPitchInput(LookInput.Y);	// Pitch 회전
}

// 점프  이벤트
void ADummy_WeaponCharacter::StartJumpEvent(const FInputActionValue& value)
{
	Jump();
}

// Shot 입력 이벤트
void ADummy_WeaponCharacter::Shot(const FInputActionValue& value)
{
	if (CurrentWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Shot!!!")));
		// 총알 스폰 위치 업데이트 및 발사
		UpdateMuzzleTransform();
		CurrentWeapon->SetActorLocation(MuzzleSocketLocation);
		CurrentWeapon->SetActorRotation(MuzzleSocketRotation);
		CurrentWeapon->ShootBullet();	// 클릭시 선택한 무기 (현재는 Shotgun) 발사
	}
}

// Reload 입력 이벤트
void ADummy_WeaponCharacter::Reload(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Reload!!!")));
}

// 1번 무기 변경 입력 이벤트
void ADummy_WeaponCharacter::ChangeWeaponOne(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("ChangeWeaponOne!!")));
	SwitchWeapon(0);	// Rifle 무기 변경
}

// 2번 무기 변경 입력 이벤트
void ADummy_WeaponCharacter::ChangeWeaponTwo(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("ChangeWeaponTwo!!")));
	SwitchWeapon(1);	// Shotgun 무기 변경
}

// 무기 변경
void ADummy_WeaponCharacter::SwitchWeapon(int32 WeaponIndex)
{
	if (WeaponInventory.IsValidIndex(WeaponIndex))
	{
		CurrentWeapon = WeaponInventory[WeaponIndex];	// 무기 변경
		if (CurrentWeapon)
		{
			CurrentWeapon->Activate();	// 무기에 맞는 Activate 실행
		}
	}
}

// 총구 위치 업데이트
void ADummy_WeaponCharacter::UpdateMuzzleTransform()
{
	MuzzleSocketLocation = GetMesh()->GetSocketLocation("Muzzle");
	MuzzleSocketRotation = GetMesh()->GetSocketRotation("Muzzle");
}
