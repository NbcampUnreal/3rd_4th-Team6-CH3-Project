#include "Weapon/GM_Weapon_Shotgun.h"
#include "Weapon/WeaponProjectile.h"

/*
	���� ���� 
	1. �Ѿ�(Projectile Ŭ����) ���� (���⼱ WeaponProjectile Ŭ������ �Ҵ�� ��)
	2. Weapon Actor TransForm ���� 
	(Weapon�� ���� SkeletalMesh�� Muzzle ���Ͽ� ������)	<- ĳ���� Ŭ�������� �ش� Ŭ����(GM_WeaponShotgunClass) ��������
	3. ShotgunSpreadAngle �� ���� �ѹ��� �������� �Ѿ��� � ������ �߻���� ����� ���� �� ���
	4. Projectile->FireIndirection(ShotDirection) WeaponProjectile Ŭ������ Projectile�� ���� �� ���⿡ ���� �߻�
*/

AGM_Weapon_Shotgun::AGM_Weapon_Shotgun()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	SetRootComponent(RootSceneComp);
	//ShotgunProjectileClass = AWeaponProjectile::StaticClass();	// Weapon ������ Projectile Ŭ���� �Ҵ� BP�� �ƴ϶� �ƹ��͵� �Ⱥ��̴°�..
}

void AGM_Weapon_Shotgun::Activate()
{
	UE_LOG(LogTemp, Warning, TEXT("����(Shotgun)"));
}

// �Ѿ� ���� 
void AGM_Weapon_Shotgun::ShootBullet()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("ShootBullet!!!")));
	if (ShotgunProjectileClass == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Class Null !!!")));
		return;	// Projectile ������ҽ� �߻� X
	}

	// źȯ ������ ��ġ�� ȸ����
	// ���� ������ ��ġ WeaponShotgun�� Character���� Muzzle���Ͽ� �پ� �����ȴ�. 
	FVector SpawnLocation = GetActorLocation();	
	FRotator SpawnRotation = GetActorRotation();

	float HalfSpread = ShotgunSpreadAngle * 0.5f;
	
	for (int32 i = 0 ; i < PelletCount ; ++i)
	{
		// �� źȯ�� ���� ���� ���� ���
		float SpreadStep = (ShotgunSpreadAngle) / (PelletCount - 1);
		// ��ä�� ���� �� ���� źȯ�� Yaw ȸ���� �Ҵ� ( -Half ~ + Half ����)
		float CurrentAngle = -HalfSpread + i * SpreadStep;

		// ���ع��⿡�� CurrentAngle ��ŭ Yaw ȸ�� �� ����
		FRotator PelletRotation = SpawnRotation;
		PelletRotation.Yaw += CurrentAngle;

		FVector ShotDirection = PelletRotation.Vector();	// �߻� ���⺤�� ����
		
		// Projectile ���� ��ġ �� ȸ�� ����
		AWeaponProjectile* Projectile =
			GetWorld()->SpawnActor<AWeaponProjectile>(ShotgunProjectileClass, SpawnLocation, SpawnRotation);

		if (Projectile)	// Projectile ������ 
		{
			Projectile->FireIndirection(ShotDirection);	// �߻� ���⿡ �°� �߻�
		}
	}
}