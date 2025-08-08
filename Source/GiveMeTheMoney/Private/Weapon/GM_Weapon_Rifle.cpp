// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GM_Weapon_Rifle.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h" // UGameplayStatics�� ����ϱ� ���� �߰�

AGM_Weapon_Rifle::AGM_Weapon_Rifle()
{
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);

    MaxAmmo = 30;
    CurrentAmmo = MaxAmmo;
    FireRatePerSecond = 5.0f;
    TimeBetweenShots = 1.0f / FireRatePerSecond;
    LastFireTime = 0.f;
    
    DamageAmount = 8.0f;
}

void AGM_Weapon_Rifle::Activate()
{
    if (CurrentAmmo <= 0 || GetWorld()->GetTimeSeconds() < LastFireTime + TimeBetweenShots)
        return;

    CurrentAmmo--;
    LastFireTime = GetWorld()->GetTimeSeconds();

    AActor* MyOwner = GetOwner();
    if (!MyOwner) return;

    AController* OwnerController = MyOwner->GetInstigatorController();
    if (!OwnerController) return;

    FVector SpawnLocation = WeaponMesh->GetSocketLocation("MuzzleSocket");
    FRotator SpawnRotation = OwnerController->GetControlRotation();

    AActor* BulletActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), SpawnLocation, SpawnRotation);
    if (BulletActor)
    {
        UStaticMeshComponent* BulletMesh = NewObject<UStaticMeshComponent>(BulletActor, TEXT("BulletMesh"));
        BulletMesh->RegisterComponent();
        BulletActor->SetRootComponent(BulletMesh);
        BulletMesh->SetStaticMesh(BulletMeshAsset);
        BulletMesh->SetWorldScale3D(FVector(0.1f));
        BulletMesh->SetCollisionProfileName(TEXT("BlockAll"));
        if (BulletMaterialAsset)
            BulletMesh->SetMaterial(0, BulletMaterialAsset);

        UProjectileMovementComponent* BulletMovement = NewObject<UProjectileMovementComponent>(BulletActor, TEXT("BulletMovement"));
        BulletMovement->RegisterComponent();
        BulletMovement->SetUpdatedComponent(BulletMesh);
        BulletMovement->InitialSpeed = 5000.f;
        BulletMovement->MaxSpeed = 5000.f;
        BulletMovement->bRotationFollowsVelocity = true;
        BulletMovement->ProjectileGravityScale = 0.f;
        
        // �浹 �߻� �� OnProjectileStop ��������Ʈ�� ���ε��Ͽ� ���� ������ �����մϴ�.
        BulletMovement->OnProjectileStop.AddDynamic(this, &AGM_Weapon_Rifle::OnBulletHit);

        BulletActor->SetLifeSpan(3.0f);
    }
    
    if (MuzzleFlashEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashEffect, SpawnLocation, SpawnRotation);
    }
}


// OnBulletHit �Լ��� ������ �����մϴ�.
void AGM_Weapon_Rifle::OnBulletHit(const FHitResult& HitResult)
{
    if (AActor* HitActor = HitResult.GetActor())
    {
        // UGameplayStatics::ApplyDamage �Լ��� ȣ���Ͽ� ���ظ� �����ϴ�.
        UGameplayStatics::ApplyDamage(
            HitActor,
            DamageAmount,
            GetInstigatorController(),
            this,
            nullptr);
    }
    
    // �浹 �� �Ѿ� ���� �ı�
    if (AActor* BulletActor = HitResult.GetActor())
    {
        BulletActor->Destroy();
    }
}



