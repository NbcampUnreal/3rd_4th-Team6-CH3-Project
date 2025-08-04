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
	     virtual void Activate();
	
 protected:
	    virtual void BeginPlay() override;
};
