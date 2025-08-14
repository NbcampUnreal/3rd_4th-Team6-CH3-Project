#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"	// ���̾ư��� �ý��� ���� �Լ�
#include "NiagaraSystem.h"	// ���̾ư��� �ý��� Ÿ��
#include "WeaponProjectile.generated.h"

class USphereComponent;	// Sphere, Projectile ���� ����
class UProjectileMovementComponent;

UCLASS()
class GIVEMETHEMONEY_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeaponProjectile();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "BulletLifeSpan")
	float ProjectileLifeSpan = 10.0f;	// źȯ ���� �ð�
	UPROPERTY(EditAnywhere, Category = "BulletDamage")
	float DamageAmount = 10.0f;	//������ 

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeshComponent")
	UStaticMeshComponent* MeshComp;	// ����ƽ �޽� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeshComponent")
	USphereComponent* CollisionComp;	// �ݸ��� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeshComponent")
	UNiagaraSystem* OverlapHitEffect;	// Hit �� ������ NiagaraSystem


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeshComponent")
	UProjectileMovementComponent* BulletProjectileMovement;	// ������Ÿ��(�߻������) ������Ʈ
	
	// Overlap �� OnHitBullet �Լ� ����
	UFUNCTION()
	void OnHitBullet(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void OnHitDamage(AActor* DamagedActor, float BaseDamage, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass );

	void FireIndirection(const FVector& ShootDirection);	// ���ư� ��������
};
