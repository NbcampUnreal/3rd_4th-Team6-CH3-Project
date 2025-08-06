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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WeaponMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMesh* BulletMeshAsset;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UMaterialInterface* BulletMaterialAsset;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float FireRatePerSecond = 5.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	int32 MaxAmmo = 30;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	int32 CurrentAmmo;
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* MuzzleFlashEffect;

private:
	float TimeBetweenShots;
	float LastFireTime;
};
