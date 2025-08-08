#pragma once

#include "CoreMinimal.h"
#include "Weapon/GM_BaseWeapon.h"
#include "Weapon/WeaponProjectile.h"	// 탄환 발사 헤더
#include "GameFramework/Actor.h"
#include "GM_Weapon_Shotgun.generated.h"

UCLASS()
class GIVEMETHEMONEY_API AGM_Weapon_Shotgun : public AGM_BaseWeapon
{
	GENERATED_BODY()

protected:
	AGM_Weapon_Shotgun();
	UPROPERTY(VisibleAnywhere, Category="RootSecne")
	USceneComponent* RootSceneComp;		// Root SceneComp
	
protected:
	//Weapon Projectile 클래스 설정
	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<class AWeaponProjectile> ShotgunProjectileClass;	// 샷건 프로젝타일 클래스 설정

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	float ShotgunSpreadAngle = 30.0f;	// 부채꼴 발사 범위 각도 (30도 -> +-15도씩 퍼질예정)

	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	int32 PelletCount = 8;	// 한번에 발사할 탄환갯수

public:	
	virtual void Activate() override;
	virtual void ShootBullet() override;
};
