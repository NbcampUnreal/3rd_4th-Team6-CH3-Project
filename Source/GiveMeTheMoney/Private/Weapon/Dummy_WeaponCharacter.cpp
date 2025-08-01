// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Dummy_WeaponCharacter.h"

// Sets default values
ADummy_WeaponCharacter::ADummy_WeaponCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADummy_WeaponCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummy_WeaponCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADummy_WeaponCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

