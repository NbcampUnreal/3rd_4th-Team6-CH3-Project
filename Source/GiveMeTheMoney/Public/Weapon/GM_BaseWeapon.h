// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GM_BaseWeapon.generated.h"

UCLASS()
class GIVEMETHEMONEY_API AGM_BaseWeapon : public AActor
{
	GENERATED_BODY()	

public:	
	AGM_BaseWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Activate();	// ���� Ȱ��ȭ �� Ÿ�Կ� �´� ����Ʈ ����
	virtual void ShootBullet();	// ���⿡ �´� źȯ �߻�
	virtual int GetAmmo() const;	// ���� ź�� ��ȯ
	virtual int GetMaxAmmo() const;	// �� ź�� ���� ��ȯ
	virtual void Reload();	// ������

	int32 MaxAmmo;	// ź�� �ִ� ����
	int32 CurrentAmmo; // ���� ź�� �� 
		
	
 protected:
	    virtual void BeginPlay() override;
};
