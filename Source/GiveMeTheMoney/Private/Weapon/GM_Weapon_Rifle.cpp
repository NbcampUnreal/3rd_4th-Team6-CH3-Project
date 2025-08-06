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
    // �� ���Ͱ� ������ ��, WeaponMesh ������Ʈ�� �����մϴ�.
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

    // WeaponMesh�� �� ������ ��Ʈ(�ֻ���) ������Ʈ�� �����մϴ�.
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

    // 2. ������ Ȯ��: �� ���⸦ ������ ĳ���Ͱ� �ִ��� Ȯ���մϴ�.
    AActor* MyOwner = GetOwner();
    if (!MyOwner) return;

    // 3. ��Ʈ�ѷ� Ȯ��: �����ڸ� �����ϴ� ��Ʈ�ѷ��� �ִ��� Ȯ���մϴ�.
    AController* OwnerController = MyOwner->GetInstigatorController();
    if (!OwnerController) return;

    // 4. �߻� ��ġ�� ���� ���
    const FVector SpawnLocation = WeaponMesh->GetSocketLocation("MuzzleSocket"); // "MuzzleSocket" ��ġ
    const FRotator SpawnRotation = OwnerController->GetControlRotation();        // ���� ����

    // 5. �� ���͸� ���忡 ����(����)�մϴ�.
    AActor* BulletActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), SpawnLocation, SpawnRotation);
    if (BulletActor)
    {
        // 6-1. ����(StaticMesh) ������Ʈ �߰�
        UStaticMeshComponent* BulletMesh = NewObject<UStaticMeshComponent>(BulletActor, TEXT("BulletMesh"));
        BulletMesh->RegisterComponent();
        BulletActor->SetRootComponent(BulletMesh);
        BulletMesh->SetCollisionProfileName(TEXT("BlockAll")); // �浹 Ȱ��ȭ
        BulletMesh->SetStaticMesh(BulletMeshAsset);            // �޽� ����
        BulletMesh->SetWorldScale3D(FVector(0.1f));             // źȯ ũ�� ����

        // ��Ƽ������ �����Ǿ� ������ ����
        if (BulletMaterialAsset)
        {
            BulletMesh->SetMaterial(0, BulletMaterialAsset);
        }

        // 6-2. �̵�(ProjectileMovement) ������Ʈ �߰�
        UProjectileMovementComponent* BulletMovement = NewObject<UProjectileMovementComponent>(BulletActor, TEXT("BulletMovement"));
        BulletMovement->RegisterComponent();
        BulletMovement->SetUpdatedComponent(BulletMesh);
        BulletMovement->InitialSpeed = 5000.f;
        BulletMovement->MaxSpeed = 5000.f;
        BulletMovement->bRotationFollowsVelocity = true; // ���� ���⿡ ���� ȸ��
        BulletMovement->ProjectileGravityScale = 0.f;    // �߷� ���� ����

        // 7. 3�� �ڿ� �ڵ����� ��������� ���� (�޸� ����)
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


    
