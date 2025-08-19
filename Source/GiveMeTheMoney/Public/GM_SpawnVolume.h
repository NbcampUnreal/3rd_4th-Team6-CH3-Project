#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GM_SpawnVolume.generated.h"

class UBoxComponent;	// Box 컴포넌트 전방선언
class AGM_AICharacter;	// AI 캐릭터 전방선언

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
	UBoxComponent* SpawningBox;	// 스폰할 범위

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AICharacterClass")
	TSubclassOf<AGM_AICharacter> AICharacterClass;

	FVector GetRandomPointInVolume() const; // 스폰 박스 안에 랜덤한 위치를 반환
	void SpawnAICharacter(int32 AICharcterCount);	// AI 캐릭터 스폰
};
