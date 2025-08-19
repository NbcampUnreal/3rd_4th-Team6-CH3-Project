#include "Weapon/WeaponProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Weapon/Dummy_WeaponCharacter.h"
#include "Kismet/GameplayStatics.h"
/*
	�Ѿ� �߻� Ŭ���� 
	�ش� Ŭ������ GM_Weapon_ShotGun Ŭ������ GM_Weapon_Rifle Ŭ�������� ������ ����
	�߻� ������ �޾� FireIndirection �Լ��� ȣ���Ͽ� �߻��
*/
AWeaponProjectile::AWeaponProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(15.0f);	// �ݸ��� ���Ǿ� ������
	CollisionComp->SetCollisionProfileName("ProjectileProfile");	//�ݸ��� �������� ����	�����Ϳ��� Projectile �����ؾ���
	SetRootComponent(CollisionComp);	//RootComponent�� ����
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponProjectile::OnHitBullet);	// Overlap�̺�Ʈ HitBullet���� ���ε�
	
	// ����ƽ�޽� ����
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));	
	MeshComp->SetupAttachment(CollisionComp);	//Collsion���� ����

	BulletProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	BulletProjectileMovement->UpdatedComponent = CollisionComp;
	BulletProjectileMovement->InitialSpeed = 1500.0f;
	BulletProjectileMovement->MaxSpeed = 1500.0f;
	BulletProjectileMovement->bRotationFollowsVelocity = true;	// �ӵ��� ���� ȸ���� ��ȭ ����
	BulletProjectileMovement->bShouldBounce = false;	// �ٿ ������

}

void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(ProjectileLifeSpan);	// źȯ ���� �ð� ����
}

void AWeaponProjectile::OnHitBullet(UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;	// �ڱ⸦ ȣ���� �θ� ����(WeaponClass)�� �������� return

	if (OtherActor != this && OtherComp && OtherActor)	// �ڱ� �ڽ��� �ƴϰ� OtherComp �� OtherActor�� �����Ҷ� 
	{
		if (!OtherActor->ActorHasTag("Player"))	// �÷��̾� ĳ���Ͱ� �ƴҶ� ����
		{
			// Hit�� ���� �α� ���
			// UE_LOG(LogTemp, Warning, TEXT("BulletHit! OtherActor : %s"), *OtherActor->GetName());
			OnHitDamage(OtherActor, DamageAmount, this, UDamageType::StaticClass());	// ������ �Լ� ȣ��
			
			//���̾ư��� ����Ʈ �����Ǿ��ִٸ�
			if (OverlapHitEffect)
			{
				// NiagaraSystem ����
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					OverlapHitEffect,
					GetActorLocation(),
					GetActorRotation()
				);
			}

			// źȯ �Ҹ�
			Destroy();
		}
	}
}

// ������ ����
void AWeaponProjectile::OnHitDamage(AActor* DamagedActor, float BaseDamage, 
	AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	//UE_LOG(LogTemp, Warning, TEXT("Called OnHitDamage"))
	UGameplayStatics::ApplyDamage(
		DamagedActor,
		BaseDamage,
		nullptr,
		DamageCauser,
		DamageTypeClass
	);
}



void AWeaponProjectile::FireIndirection(const FVector& ShootDirection)
{
	// ���� * Projectile �ӵ� ���Ͽ� ���ư� ���⿡ �´� �ӵ��� ��ȯ
	BulletProjectileMovement->Velocity = ShootDirection * BulletProjectileMovement->InitialSpeed;
}

