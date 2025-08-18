#include "GM_SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "GM_AICharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGM_SpawnVolume::AGM_SpawnVolume()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);	// ��Ʈ Scene ����

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);	// Scene���� ���̱�
	SpawningBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// �浹 X
	
}

// Called when the game starts or when spawned
void AGM_SpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	//SpawnAICharacter(3);	// 3���� ����
}

FVector AGM_SpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();	// Box ũ�⸦ ����
	FVector BoxOrigin = SpawningBox->GetComponentLocation();	// �ڽ��� �߽���ġ

	return UKismetMathLibrary::RandomPointInBoundingBox(BoxOrigin, BoxExtent);	// ���� �ڽ��� ���� ��ġ ��ȯ
}

void AGM_SpawnVolume::SpawnAICharacter(int32 AICharcterCount)
{
	if (!AICharacterClass) return;	// AI ĳ���� ������ ���� X
	
	for(int32 i = 0 ; i < AICharcterCount ; ++i)
	{
		GetWorld()->SpawnActor<AGM_AICharacter>(
			AICharacterClass,
			GetRandomPointInVolume(),
			FRotator::ZeroRotator
		);

		Destroy();	// ���� ���� �� ���� ���� ����
	}
}
