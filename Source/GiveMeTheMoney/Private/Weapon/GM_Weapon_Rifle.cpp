// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GM_Weapon_Rifle.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapon/WeaponProjectile.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h" // UGameplayStatics�� ����ϱ� ���� �߰�

AGM_Weapon_Rifle::AGM_Weapon_Rifle()
{
    // SceneComponent �� Root�� ����
    RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
    SetRootComponent(RootSceneComp);
   /* WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);

    MaxAmmo = 30;
    CurrentAmmo = MaxAmmo;
    FireRatePerSecond = 5.0f;
    TimeBetweenShots = 1.0f / FireRatePerSecond;
    LastFireTime = 0.f;
    
    DamageAmount = 8.0f;*/
}

void AGM_Weapon_Rifle::Activate()
{
   
}

// �Ѿ� �߻� 
void AGM_Weapon_Rifle::ShootBullet()
{
    if (RifleProjectileClass == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("RifleClass Null !!!")));
        return;
    }
    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = GetActorRotation();

    // �Ѿ� ����
    AWeaponProjectile* Projectile =
        GetWorld()->SpawnActor<AWeaponProjectile>(RifleProjectileClass, SpawnLocation, SpawnRotation);
    if (Projectile)
    {
        Projectile->FireIndirection(GetActorForwardVector());   //���� ������ �������� �Ѿ� �߻�
    }
}



