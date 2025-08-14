// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "Weapon/GM_BaseWeapon.h"
#include "GM_Weapon_Rifle.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;

UCLASS()
class GIVEMETHEMONEY_API AGM_Weapon_Rifle : public AGM_BaseWeapon
{
	GENERATED_BODY()

public:
	AGM_Weapon_Rifle();

	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<class AWeaponProjectile> RifleProjectileClass;	// 라이플 클래스

	UPROPERTY(VisibleAnywhere, Category = "RootSecne")
	USceneComponent* RootSceneComp;		// Root SceneComp

	virtual void Activate() override;
	virtual void ShootBullet() override;	//총알 발사
	virtual void Reload() override;

private:
	float TimeBetweenShots;
	float LastFireTime;
};
