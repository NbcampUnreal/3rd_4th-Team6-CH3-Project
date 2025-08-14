#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"	// 나이아가라 시스템 스폰 함수
#include "NiagaraSystem.h"	// 나이아가라 시스템 타입
#include "WeaponProjectile.generated.h"

class USphereComponent;	// Sphere, Projectile 전방 선언
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
	float ProjectileLifeSpan = 10.0f;	// 탄환 생존 시간
	UPROPERTY(EditAnywhere, Category = "BulletDamage")
	float DamageAmount = 10.0f;	//데미지 

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeshComponent")
	UStaticMeshComponent* MeshComp;	// 스태틱 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeshComponent")
	USphereComponent* CollisionComp;	// 콜리전 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeshComponent")
	UNiagaraSystem* OverlapHitEffect;	// Hit 시 스폰할 NiagaraSystem


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeshComponent")
	UProjectileMovementComponent* BulletProjectileMovement;	// 프로젝타일(발사움직임) 컴포넌트
	
	// Overlap 시 OnHitBullet 함수 실행
	UFUNCTION()
	void OnHitBullet(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void OnHitDamage(AActor* DamagedActor, float BaseDamage, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass );

	void FireIndirection(const FVector& ShootDirection);	// 나아갈 방향조절
};
