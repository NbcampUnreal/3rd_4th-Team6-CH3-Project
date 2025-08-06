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
	virtual void Activate() override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WeaponMesh;

	// 블루프린트에서 총알의 StaticMesh를 설정할 수 있도록 합니다.
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* BulletMeshAsset;

	// 블루프린트에서 총알의 Material을 설정할 수 있도록 합니다.
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* BulletMaterialAsset;

	UPROPERTY(EditDefaultsOnly)
	float FireRatePerSecond;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmo;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* MuzzleFlashEffect;
    
	UPROPERTY(EditDefaultsOnly)
	float DamageAmount;

	// OnBulletHit 함수를 protected 섹션에 선언합니다.
	UFUNCTION()
	void OnBulletHit(const FHitResult& HitResult);

private:
	float TimeBetweenShots;
	float LastFireTime;
};
