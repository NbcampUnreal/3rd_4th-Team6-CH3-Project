#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GM_AIController.generated.h"


UCLASS()
class GIVEMETHEMONEY_API AGM_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AGM_AIController();

	static const FName PatrolPosKey;
	static const FName HomePosKey;
	static const FName TargetKey;

protected:
	UPROPERTY()
	UBlackboardData* BBAsset;
	UPROPERTY()
	UBehaviorTree* BTAsset;

	virtual void BeginPlay() override;
	void GMCHECK(bool bCondition);
	void OnRepeatTimer();
	virtual void OnPossess(APawn* InPawn) override;
	void OnUnPossess();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
};

