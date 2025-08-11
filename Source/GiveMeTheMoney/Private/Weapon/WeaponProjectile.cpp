#include "Weapon/WeaponProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
/*
	총알 발사 클래스 
	해당 클래스는 GM_Weapon_ShotGun 클래스와 GM_Weapon_Rifle 클래스에서 스폰될 예정
	발사 방향을 받아 FireIndirection 함수를 호출하여 발사됨
*/
AWeaponProjectile::AWeaponProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(15.0f);	// 콜리전 스피어 반지름
	CollisionComp->SetCollisionProfileName("Projectile");	//콜리전 프로파일 설정	에디터에서 Projectile 생성해야함
	SetRootComponent(CollisionComp);	//RootComponent로 설정
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponProjectile::OnHitBullet);	// Overlap이벤트 HitBullet으로 바인딩
	
	// 스태틱메쉬 생성
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));	
	MeshComp->SetupAttachment(CollisionComp);	//Collsion으로 붙임

	BulletProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	BulletProjectileMovement->UpdatedComponent = CollisionComp;
	BulletProjectileMovement->InitialSpeed = 1500.0f;
	BulletProjectileMovement->MaxSpeed = 1500.0f;
	BulletProjectileMovement->bRotationFollowsVelocity = true;	// 속도에 따른 회전값 변화 적용
	BulletProjectileMovement->bShouldBounce = false;	// 바운스 미적용

	InitialLifeSpan = 10.0f;	// 해당 클래스 생존 시간
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
	if (OtherActor == GetOwner()) return;	// 자기를 호출한 부모 액터와 오버랩시 return

	if (OtherActor != this && OtherComp && OtherActor)	// 자기 자신이 아니고 OtherComp 및 OtherActor가 존재할때 
	{
		// Hit된 액터 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("BulletHit! OtherActor : %s"), *OtherActor->GetName());	
	}
}

void AWeaponProjectile::FireIndirection(const FVector& ShootDirection)
{
	// 방향 * Projectile 속도 곱하여 나아갈 방향에 맞는 속도로 변환
	BulletProjectileMovement->Velocity = ShootDirection * BulletProjectileMovement->InitialSpeed;
}

