#include "Weapon/GM_Weapon_Shotgun.h"
#include "Weapon/WeaponProjectile.h"

/*
	샷건 웨폰 
	1. 총알(Projectile 클래스) 설정 (여기선 WeaponProjectile 클래스만 할당될 것)
	2. Weapon Actor TransForm 저장 
	(Weapon이 현재 SkeletalMesh의 Muzzle 소켓에 스폰됨)	<- 캐릭터 클래스에서 해당 클래스(GM_WeaponShotgunClass) 스폰예정
	3. ShotgunSpreadAngle 에 맞춰 한번에 여러발의 총알이 어떤 각도로 발사될지 방향과 각도 값 계산
	4. Projectile->FireIndirection(ShotDirection) WeaponProjectile 클래스인 Projectile를 생성 후 방향에 맞춰 발사
*/

AGM_Weapon_Shotgun::AGM_Weapon_Shotgun()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	SetRootComponent(RootSceneComp);
	//ShotgunProjectileClass = AWeaponProjectile::StaticClass();	// Weapon 생성시 Projectile 클래스 할당 BP가 아니라서 아무것도 안보이는거..
}

void AGM_Weapon_Shotgun::Activate()
{
	UE_LOG(LogTemp, Warning, TEXT("샷건(Shotgun)"));
}

// 총알 스폰 
void AGM_Weapon_Shotgun::ShootBullet()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("ShootBullet!!!")));
	if (ShotgunProjectileClass == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Class Null !!!")));
		return;	// Projectile 존재안할시 발사 X
	}

	// 탄환 스폰할 위치와 회전값
	// 현재 액터의 위치 WeaponShotgun이 Character에서 Muzzle소켓에 붙어 스폰된다. 
	FVector SpawnLocation = GetActorLocation();	
	FRotator SpawnRotation = GetActorRotation();

	float HalfSpread = ShotgunSpreadAngle * 0.5f;
	
	for (int32 i = 0 ; i < PelletCount ; ++i)
	{
		// 각 탄환이 퍼질 각도 간격 계산
		float SpreadStep = (ShotgunSpreadAngle) / (PelletCount - 1);
		// 부채꼴 범위 내 현재 탄환의 Yaw 회전값 할당 ( -Half ~ + Half 까지)
		float CurrentAngle = -HalfSpread + i * SpreadStep;

		// 조준방향에서 CurrentAngle 만큼 Yaw 회전 값 적용
		FRotator PelletRotation = SpawnRotation;
		PelletRotation.Yaw += CurrentAngle;

		FVector ShotDirection = PelletRotation.Vector();	// 발사 방향벡터 구함
		
		// Projectile 생성 위치 및 회전 지정
		AWeaponProjectile* Projectile =
			GetWorld()->SpawnActor<AWeaponProjectile>(ShotgunProjectileClass, SpawnLocation, SpawnRotation);

		if (Projectile)	// Projectile 스폰시 
		{
			Projectile->FireIndirection(ShotDirection);	// 발사 방향에 맞게 발사
		}
	}
}