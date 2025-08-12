#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GM_BTS_Detect.generated.h"

UCLASS()
class GIVEMETHEMONEY_API UGM_BTS_Detect : public UBTService
{
	GENERATED_BODY()

public:
	UGM_BTS_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	float TimeSinceLastSeen;
	float LostTargetDelay;
};