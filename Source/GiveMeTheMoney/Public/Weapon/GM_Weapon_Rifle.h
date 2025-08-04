// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GM_BaseWeapon.h"
#include "GameFramework/Actor.h"
#include "GM_Weapon_Rifle.generated.h"

UCLASS()
class GIVEMETHEMONEY_API AGM_Weapon_Rifle : public AGM_BaseWeapon
{
	GENERATED_BODY()
	
public:	
	virtual void Activate() override;
};
