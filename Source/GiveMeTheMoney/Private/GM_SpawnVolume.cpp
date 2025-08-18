#include "GM_SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "GM_AICharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGM_SpawnVolume::AGM_SpawnVolume()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);	// 루트 Scene 설정

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);	// Scene으로 붙이기
	SpawningBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 충돌 X
	
}

// Called when the game starts or when spawned
void AGM_SpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	//SpawnAICharacter(3);	// 3마리 스폰
}

FVector AGM_SpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();	// Box 크기를 얻어옴
	FVector BoxOrigin = SpawningBox->GetComponentLocation();	// 박스의 중심위치

	return UKismetMathLibrary::RandomPointInBoundingBox(BoxOrigin, BoxExtent);	// 스폰 박스의 랜덤 위치 반환
}

void AGM_SpawnVolume::SpawnAICharacter(int32 AICharcterCount)
{
	if (!AICharacterClass) return;	// AI 캐릭터 없으면 실행 X
	
	for(int32 i = 0 ; i < AICharcterCount ; ++i)
	{
		GetWorld()->SpawnActor<AGM_AICharacter>(
			AICharacterClass,
			GetRandomPointInVolume(),
			FRotator::ZeroRotator
		);

		Destroy();	// 스폰 끝난 후 스폰 볼륨 제거
	}
}
