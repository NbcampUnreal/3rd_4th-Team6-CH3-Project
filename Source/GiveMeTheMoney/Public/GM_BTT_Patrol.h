#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GM_BTT_Patrol.generated.h"

UCLASS()
class GIVEMETHEMONEY_API UGM_BTT_Patrol : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGM_BTT_Patrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
