// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GM_BaseWeapon.h"

// Sets default values
AGM_BaseWeapon::AGM_BaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGM_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGM_BaseWeapon::Activate()
{
}
