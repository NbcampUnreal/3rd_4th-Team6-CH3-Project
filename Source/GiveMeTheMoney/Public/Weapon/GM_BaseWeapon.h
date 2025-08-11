// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GM_BaseWeapon.generated.h"

UCLASS()
class GIVEMETHEMONEY_API AGM_BaseWeapon : public AActor
{
	GENERATED_BODY()	

public:	
	AGM_BaseWeapon();

	     UFUNCTION(BlueprintCallable, Category = "Weapon")
	     virtual void Activate();	// 무기 활성화 시 타입에 맞는 이펙트 스폰
		 virtual void ShootBullet();	// 무기에 맞는 탄환 발사
	
 protected:
	    virtual void BeginPlay() override;
};
