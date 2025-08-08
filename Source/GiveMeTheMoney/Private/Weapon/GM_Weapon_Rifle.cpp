// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GM_Weapon_Rifle.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h" // UGameplayStatics를 사용하기 위해 추가

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
        
        // 충돌 발생 시 OnProjectileStop 델리게이트를 바인딩하여 피해 로직을 실행합니다.
        BulletMovement->OnProjectileStop.AddDynamic(this, &AGM_Weapon_Rifle::OnBulletHit);

        BulletActor->SetLifeSpan(3.0f);
    }
    
    if (MuzzleFlashEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashEffect, SpawnLocation, SpawnRotation);
    }
}


// OnBulletHit 함수를 별도로 정의합니다.
void AGM_Weapon_Rifle::OnBulletHit(const FHitResult& HitResult)
{
    if (AActor* HitActor = HitResult.GetActor())
    {
        // UGameplayStatics::ApplyDamage 함수를 호출하여 피해를 입힙니다.
        UGameplayStatics::ApplyDamage(
            HitActor,
            DamageAmount,
            GetInstigatorController(),
            this,
            nullptr);
    }
    
    // 충돌 후 총알 액터 파괴
    if (AActor* BulletActor = HitResult.GetActor())
    {
        BulletActor->Destroy();
    }
}



