#include "Weapon/WeaponProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
/*
	�Ѿ� �߻� Ŭ���� 
	�ش� Ŭ������ GM_Weapon_ShotGun Ŭ������ GM_Weapon_Rifle Ŭ�������� ������ ����
	�߻� ������ �޾� FireIndirection �Լ��� ȣ���Ͽ� �߻��
*/
AWeaponProjectile::AWeaponProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(15.0f);	// �ݸ��� ���Ǿ� ������
	CollisionComp->SetCollisionProfileName("Projectile");	//�ݸ��� �������� ����	�����Ϳ��� Projectile �����ؾ���
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

	InitialLifeSpan = 10.0f;	// �ش� Ŭ���� ���� �ð�
}

void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("SpawnProjectile!!!")));
}

void AWeaponProjectile::OnHitBullet(UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;	// �ڱ⸦ ȣ���� �θ� ���Ϳ� �������� return

	if (OtherActor != this && OtherComp && OtherActor)	// �ڱ� �ڽ��� �ƴϰ� OtherComp �� OtherActor�� �����Ҷ� 
	{
		// Hit�� ���� �α� ���
		UE_LOG(LogTemp, Warning, TEXT("BulletHit! OtherActor : %s"), *OtherActor->GetName());	
	}
}

void AWeaponProjectile::FireIndirection(const FVector& ShootDirection)
{
	// ���� * Projectile �ӵ� ���Ͽ� ���ư� ���⿡ �´� �ӵ��� ��ȯ
	BulletProjectileMovement->Velocity = ShootDirection * BulletProjectileMovement->InitialSpeed;
}

