#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GM_BTT_ChaseTarget.generated.h"

UCLASS()
class GIVEMETHEMONEY_API UGM_BTT_ChaseTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGM_BTT_ChaseTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
