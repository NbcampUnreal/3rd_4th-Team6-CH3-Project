// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/GM_BaseWeapon.h"
#include "GM_Character.generated.h"

class USpringArmComponent;// 미리 선언
class UCameraComponent;// 스프링 암, 카메라 컴포넌트 클래스를 사용할 수 있도록 선언
class UInputMappingContext;
class UInputAction;
class AGM_GameModeBase;
struct FInputActionValue;// Move 입력 값을 받기 위한 구조체 선언

UCLASS()
class GIVEMETHEMONEY_API AGM_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGM_Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")// 내부 속성은 조정 가능함
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")// 내부 속성은 조정 가능함
		UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<AGM_BaseWeapon*> WeaponInventory;	// 무기 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AGM_BaseWeapon* CurrentWeapon;	// 무기
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AGM_Weapon_Rifle> Rifle;	//WeaponRifle 클래스만 할당
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AGM_Weapon_Shotgun> Shotgun;	//WeaponShotgun 클래스만 할당

	// 총구 위치 ,회전값
	FVector MuzzleSocketLocation;
	FRotator MuzzleSocketRotation;

	UFUNCTION(BlueprintPure, Category = "Health")// 블루프린트에서 읽기 전용으로 사용 가능
	float GetHealth() const;	// 현재 체력을 반환하는 함수
		
	float GetMaxHealth() const;	// 최대 체력 반환
	UFUNCTION(BlueprintCallable, Category = "Health")// 블루프린트에서 읽기 전용으로 사용 가능
		void AddHealth(float Amount);// 체력을 회복시키는 함수

	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void StartCrouch(); // 웅크리기 시작
	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void StopCrouch();  // 웅크리기 멈춤

	void UpdateMovementSpeed(float Speed);
	void StopMovement(bool IsStop);		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; // 일반적인 걷기 속도

protected:
	virtual void BeginPlay() override;

	AGM_GameModeBase* MyGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")// 에디터에서 조정 가능
		float MaxHealth;// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")// 에디터에서 조정 가능
		float Health;// 현재 체력

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;// 입력 이벤트를 처리할 함수

	virtual float TakeDamage(
		float DamageAmount,// 데미지 양
		struct FDamageEvent const& DamageEvent,    // 데미지 이벤트 구조체 스킬 시스템이나 다른 시스템에서 데미지 이벤트를 처리할 때 사용
		AController* EventInstigator,// 데미지를 입힌 주체 (플레이어, AI 등)
		AActor* DamageCauser// 데미지를 입힌 원인 (무기, 폭발 등)
	) override;// 데미지를 받았을 때 호출되는 함수 (기본 구현은 0을 반환, 자식 클래스에서 오버라이드 가능

	UFUNCTION()
	//FInputActionValue& 참조자를 통해 구조체의 데이터를 그대로 가져옴(복사와 달리 데이터 최적화 용이) Const로 원본 보호
	void Move(const FInputActionValue& value);// IA_Move 의 입력 값(Vector2D로 X,Y축에 대한)을 받음

	UFUNCTION()
	// Jump 와 같은 On/Off 동작들은 Start와 Stop 변수로 나누어서 관리 해준다.
	void StartJump(const FInputActionValue& value);//Jump 는 눌렀을 때/ 떼어냈을 때를 구분
	UFUNCTION()
	void StopJump(const FInputActionValue& value);//Jump 떼어냈을 때
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);// 질주 시작
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);// 질주 멈춤
	UFUNCTION()
	void Look(const FInputActionValue& value);// 마우스 입력(시선 처리)
	UFUNCTION()
	void Fire(const FInputActionValue& value);// 총알 발사
	UFUNCTION()
	void Reload(const FInputActionValue& value);	// 재장전
	
	void Crouch(const FInputActionValue& value);	//앉기
	
	// 웅크리기 상태를 추적할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	bool bIsCrouching = false; // 현재 웅크린 상태인지

	void OnDeath();// 캐릭터가 죽었을 때 호출되는 함수



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed; // 질주 속도

	// 무기 교체 함수
	UFUNCTION()
	void EquipWeapon1(const FInputActionValue& value); // 1번 무기 장착
	UFUNCTION()
	void EquipWeapon2(const FInputActionValue& value); // 2번 무기 장착

	void UpdateMuzzleTransform();	// 총구 위치 업데이트

private:
	//float NormalSpeed;// 현재의 일반 속도
	float SprintSpeedMultiplier;// Normal에서 몇배를 곱해줄지 설정하는 변수
	//float SprintSpeed;// 질주 속도 (Normal * Multiplier)
};