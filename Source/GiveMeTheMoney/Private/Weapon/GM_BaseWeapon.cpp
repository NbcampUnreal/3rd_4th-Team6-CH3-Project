#include "Weapon/GM_BaseWeapon.h"

// Sets default values
AGM_BaseWeapon::AGM_BaseWeapon()
{
}

// Called when the game starts or when spawned
void AGM_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGM_BaseWeapon::Activate()
{
}

void AGM_BaseWeapon::ShootBullet()
{

}

int AGM_BaseWeapon::GetAmmo() const	// ���� ź�� ��ȯ
{
	return CurrentAmmo;
}

int AGM_BaseWeapon::GetMaxAmmo() const	// �� ź�� ��ȯ
{
	return MaxAmmo;
}

void AGM_BaseWeapon::Reload()
{

}