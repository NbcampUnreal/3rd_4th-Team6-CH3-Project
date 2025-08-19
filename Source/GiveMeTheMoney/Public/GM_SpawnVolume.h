#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GM_SpawnVolume.generated.h"

class UBoxComponent;	// Box ������Ʈ ���漱��
class AGM_AICharacter;	// AI ĳ���� ���漱��

UCLASS()
class GIVEMETHEMONEY_API AGM_SpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGM_SpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SeneComp")
	USceneComponent* Scene;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawningBox")
	UBoxComponent* SpawningBox;	// ������ ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AICharacterClass")
	TSubclassOf<AGM_AICharacter> AICharacterClass;

	FVector GetRandomPointInVolume() const; // ���� �ڽ� �ȿ� ������ ��ġ�� ��ȯ
	void SpawnAICharacter(int32 AICharcterCount);	// AI ĳ���� ����
};
