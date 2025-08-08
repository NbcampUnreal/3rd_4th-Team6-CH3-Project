// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Character.h"
#include "GM_Controller_Character.h"
#include "EnhancedInputComponent.h"		// EnhancedInput 바인딩을 사용하기 위한 헤더파일
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"	// 캐릭터 무브먼트 컴포넌트 헤더파일


AGM_Character::AGM_Character()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);	//루트 컴포넌트 어태치
	SpringArmComp->TargetArmLength = 300.0f;		// 스프링 암의 길이
	SpringArmComp->bUsePawnControlRotation = true;	// 컨트롤러로 스프링 암 회전 사용

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);	//카메라 컴포넌트를 스프링암 끝 지점 부착
	CameraComp->bUsePawnControlRotation = false;	// 컨트롤러로 카메라 회전 못하게 설정

	NormalSpeed = 600.0f;	// 현재 속도 
	SprintSpeedMultiplier = 2.0f;	// 곱해질 크기
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	MaxHealth = 100.0f;	// 최대 체력 설정
	Health = MaxHealth;	// 현재 체력 설정

}


void AGM_Character::Move(const FInputActionValue& value)	// value : 2D vector 값으로 들어옴
{
	if (!Controller)	return;	//캐릭터가 유효한지 확인 (존재하지 않다면 return)

	const FVector2D MoveInput = value.Get<FVector2D>();		//value 값을 2DVector로 입력 받아 MoveInput에 저장

	if (!FMath::IsNearlyZero(MoveInput.X))	// 입력 값이 0이 아니라면
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);	//해당 액터의 정면으로 X만큼 이동
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);	// 해당 액터의 오른쪽으로 Y만큼 이동
	}
}

void AGM_Character::Look(const FInputActionValue& value)	// 시선 처리를 위한 마우스 좌표 값으로 2D Vector로 값을 받아옴
{
	FVector2D LookInput = value.Get<FVector2D>();	// 2D Vecotr 값을 저장

	AddControllerYawInput(LookInput.X);		// 좌, 우 입력
	AddControllerPitchInput(-LookInput.Y);	// 상, 하 입력
}

void AGM_Character::StartJump(const FInputActionValue& value) // value : Digit 값으로 true/false 값을 가지고 있음
{
	if (value.Get<bool>())	//Jump입력이 True일 때 StartJump 함수 실행
	{
		Jump();		// 언리얼 기본 제공 캐릭터 점프 기능
	}
}

void AGM_Character::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())	//Jump입력이 False일 때 StopJump 함수 실행
	{
		StopJumping();		// 언리얼 기본 제공 캐릭터 점프 멈추는 기능
	}
}

void AGM_Character::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;		// 가속 키 입력시 즉시 Sprint속도로 변환
	}
}

void AGM_Character::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{

		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;		// 가속 키 떼었을 때 즉시 Normal속도로 변환
	}
}

// 입력 받은 값을 어떻게 처리 할 지 구현하는 곳
void AGM_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//현재 캐릭터의 컨트롤러를 가져온 후 SpartaPlayerController로 캐스팅
		if (AGM_Controller_Character* PlayerController = Cast<AGM_Controller_Character>(GetController()))
		{
			if (PlayerController->MoveAction)	// W,A,S,D 움직임 입력 처리
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,	// 키가 입력 되었을 때
					this,						// 호출된 객체
					&AGM_Character::Move		// 객체의 Move 함수를 연결
				);
			}

			if (PlayerController->LookAction)	// 카메라 시선 입력 처리
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,	// 키가 입력 되었을 때
					this,						// 호출된 객체
					&AGM_Character::Look		// 객체의 Look 함수를 연결
				);
			}

			if (PlayerController->JumpAction)	//점프 입력 처리
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,	// 키가 입력 되었을 때
					this,						// 호출된 객체
					&AGM_Character::StartJump		// 점프 키를 눌렀을 때 입력 처리
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,	// 키를 뗀 순간 StopJump 호출
					this,						// 호출된 객체
					&AGM_Character::StopJump		// 점프 키를 떼었을 때 입력 처리
				);
			}

			if (PlayerController->SprintAction)		// 질주 입력 처리
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,	// 키가 입력 되었을 때
					this,						// 호출된 객체
					&AGM_Character::StartSprint		// 질주 키를 눌렀을 때 입력 처리
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,	// 키를 뗀 순간 StopSprint 호출
					this,						// 호출된 객체
					&AGM_Character::StopSprint	// 질주 키를 떼었을 때 입력 처리
				);
			}
		}
	}
}

float AGM_Character::GetHealth() const	// 현재 체력을 반환하는 함수
{
	return Health;
}

void AGM_Character::AddHealth(float Amount) {	// 체력을 증가시키는 함수
	if (Amount <= 0.0f) return;	// 증가할 양이 0 이하라면 아무것도 하지 않음

	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);	// 현재 체력을 증가시키고 0과 MaxHealth 사이로 제한
	UE_LOG(LogTemp, Warning, TEXT("Health increased to: %f"), Health);	// 현재 체력 로그 출력
}

float AGM_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	// ActualDamage는 부모 클래스의 TakeDamage를 호출하여 기본 데미지 처리 로직을 가져온다.
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	// 부모 클래스의 TakeDamage 호출

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);	// 현재 체력을 감소시키고 0과 MaxHealth 사이로 제한
	UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);	// 현재 체력 로그 출력

	if (Health <= 0.0f)
	{
		OnDeath();	// 체력이 0 이하가 되면 OnDeath 함수를 호출
	}

	return 0.0f;
}

void AGM_Character::OnDeath()
{
	// 게임 종료 로직 구현 예정

}