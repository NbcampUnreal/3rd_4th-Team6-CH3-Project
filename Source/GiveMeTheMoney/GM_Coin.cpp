// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Coin.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GM_GameModeBase.h"

// Sets default values
AGM_Coin::AGM_Coin()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetCollisionProfileName("OverlapOnlyPawn");

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	CoinMesh->SetupAttachment(RootComponent);
	CoinMesh->SetCollisionProfileName("NoCollision");
}

// Called when the game starts or when spawned
void AGM_Coin::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AGM_Coin::OnOverlapBegin);
}

// Called every frame
void AGM_Coin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGM_Coin::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (AGM_GameModeBase* MyGameMode = Cast<AGM_GameModeBase>(GameMode))
	{
		if (OtherActor != this && OtherComp && OtherActor)	// 다른 액터가 자기자신 아니고 컴포넌트, 액터가 오버랩되었을때
		{
			if (OtherActor->ActorHasTag("Player"))	// 플레이어 오버랩 시 
			{
				MyGameMode->OnCoinCollected();
				Destroy();
			}
		}
	}
}

