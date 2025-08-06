//시연용 더미 캐릭터 헤더
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/GM_BaseWeapon.h"
#include "Dummy_WeaponCharacter.generated.h"

class USpringArmComponent;	// 스프링암 컴포넌트 전방선언
class UCameraComponent;	// 카메라 컴포넌트 전방선언
struct FInputActionValue;	// 인풋액션 전방선언

UCLASS()
class GIVEMETHEMONEY_API ADummy_WeaponCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADummy_WeaponCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dummy_WeaponCharcter")
	USpringArmComponent* SpringArmComp;	// 스프링암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dummy_WeaponCharcter")
	UCameraComponent* CameraComp;	// 카메라 컴포넌트

	TArray<AGM_BaseWeapon*> WeaponInventory;	// 무기 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AGM_BaseWeapon* CurrentWeapon;	// 무기
	AGM_BaseWeapon* Rifle;	
	AGM_BaseWeapon* Shotgun;


protected:
	// 입력 바인딩 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Input
public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& value);	// 이동을 담당할 입력 이벤트
	UFUNCTION()
	void Look(const FInputActionValue& value);	// 시선 입력 이벤트
	UFUNCTION()
	void StartJumpEvent(const FInputActionValue& value);	// 점프 이벤트
	UFUNCTION()
	void Shot(const FInputActionValue& value);	// 무기 공격 이벤트
	UFUNCTION()
	void Reload(const FInputActionValue& value);	// 재장전 이벤트
	UFUNCTION()
	void ChangeWeaponOne(const FInputActionValue& value);	// 1번 무기 변경
	UFUNCTION()
	void ChangeWeaponTwo(const FInputActionValue& value);	// 2번 무기 변경

public:
	
	void SwitchWeapon(int32 WeaponIndex);	// 무기 변경
	
	UPROPERTY(EditAnywhere, Category = "Dummy_WeaponCharcter")
	float MoveSpeed;	// 이동속도
};
