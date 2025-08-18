// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GM_Weapon_Dummy_SpawnActor.h"

// Sets default values
AGM_Weapon_Dummy_SpawnActor::AGM_Weapon_Dummy_SpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGM_Weapon_Dummy_SpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGM_Weapon_Dummy_SpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

