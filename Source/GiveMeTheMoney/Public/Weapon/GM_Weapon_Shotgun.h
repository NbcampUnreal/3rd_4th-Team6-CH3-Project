#pragma once

#include "CoreMinimal.h"
#include "Weapon/GM_BaseWeapon.h"
#include "Weapon/WeaponProjectile.h"	// źȯ �߻� ���
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
	//Weapon Projectile Ŭ���� ����
	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<class AWeaponProjectile> ShotgunProjectileClass;	// ���� ������Ÿ�� Ŭ���� ����

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	float ShotgunSpreadAngle = 30.0f;	// ��ä�� �߻� ���� ���� (30�� -> +-15���� ��������)

	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	int32 PelletCount = 8;	// �ѹ��� �߻��� źȯ����

public:	
	virtual void Activate() override;
	virtual void ShootBullet() override;
};
