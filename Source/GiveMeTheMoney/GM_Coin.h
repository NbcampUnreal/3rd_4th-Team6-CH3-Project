// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GM_Coin.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class GIVEMETHEMONEY_API AGM_Coin : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGM_Coin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* CoinMesh;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};