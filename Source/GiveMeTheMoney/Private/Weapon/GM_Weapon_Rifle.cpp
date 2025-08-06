// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GM_Weapon_Rifle.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h" 


AGM_Weapon_Rifle::AGM_Weapon_Rifle()
{
    // 이 액터가 생성될 때, WeaponMesh 컴포넌트를 생성합니다.
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

    // WeaponMesh를 이 액터의 루트(최상위) 컴포넌트로 설정합니다.
    SetRootComponent(WeaponMesh);

    MaxAmmo = 30;
    CurrentAmmo = MaxAmmo;
    FireRatePerSecond = 5.0f;

    TimeBetweenShots = 1.0f / FireRatePerSecond;
    LastFireTime = 0.0f;
}

void AGM_Weapon_Rifle::Activate()
{
    if (CurrentAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("ReLoad!"));
        return;
    }

    if (GetWorld()->GetTimeSeconds() < LastFireTime + TimeBetweenShots)
    {
        return;
    }

    LastFireTime = GetWorld()->GetTimeSeconds();
    CurrentAmmo--;
    UE_LOG(LogTemp, Log, TEXT("CurrentAmmo: %d / %d"), CurrentAmmo, MaxAmmo);

    if (!BulletMeshAsset)
    {
        return;
    }

    // 2. 소유자 확인: 이 무기를 소유한 캐릭터가 있는지 확인합니다.
    AActor* MyOwner = GetOwner();
    if (!MyOwner) return;

    // 3. 컨트롤러 확인: 소유자를 조종하는 컨트롤러가 있는지 확인합니다.
    AController* OwnerController = MyOwner->GetInstigatorController();
    if (!OwnerController) return;

    // 4. 발사 위치와 방향 계산
    const FVector SpawnLocation = WeaponMesh->GetSocketLocation("MuzzleSocket"); // "MuzzleSocket" 위치
    const FRotator SpawnRotation = OwnerController->GetControlRotation();        // 조준 방향

    // 5. 빈 액터를 월드에 생성(스폰)합니다.
    AActor* BulletActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), SpawnLocation, SpawnRotation);
    if (BulletActor)
    {
        // 6-1. 외형(StaticMesh) 컴포넌트 추가
        UStaticMeshComponent* BulletMesh = NewObject<UStaticMeshComponent>(BulletActor, TEXT("BulletMesh"));
        BulletMesh->RegisterComponent();
        BulletActor->SetRootComponent(BulletMesh);
        BulletMesh->SetCollisionProfileName(TEXT("BlockAll")); // 충돌 활성화
        BulletMesh->SetStaticMesh(BulletMeshAsset);            // 메시 설정
        BulletMesh->SetWorldScale3D(FVector(0.1f));             // 탄환 크기 조절

        // 머티리얼이 설정되어 있으면 적용
        if (BulletMaterialAsset)
        {
            BulletMesh->SetMaterial(0, BulletMaterialAsset);
        }

        // 6-2. 이동(ProjectileMovement) 컴포넌트 추가
        UProjectileMovementComponent* BulletMovement = NewObject<UProjectileMovementComponent>(BulletActor, TEXT("BulletMovement"));
        BulletMovement->RegisterComponent();
        BulletMovement->SetUpdatedComponent(BulletMesh);
        BulletMovement->InitialSpeed = 5000.f;
        BulletMovement->MaxSpeed = 5000.f;
        BulletMovement->bRotationFollowsVelocity = true; // 진행 방향에 따라 회전
        BulletMovement->ProjectileGravityScale = 0.f;    // 중력 영향 제거

        // 7. 3초 뒤에 자동으로 사라지도록 설정 (메모리 관리)
        BulletActor->SetLifeSpan(3.0f);
    }

    if (MuzzleFlashEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            MuzzleFlashEffect,
            SpawnLocation,
            SpawnRotation
        );
    }
}


    
