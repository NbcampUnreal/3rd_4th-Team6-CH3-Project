#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GM_BTT_Attack.generated.h"

UCLASS()
class GIVEMETHEMONEY_API UGM_BTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGM_BTT_Attack();

	bool IsAttacking = false;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	
	UFUNCTION()
	void OnAttackEnded();
	UBehaviorTreeComponent* OwnerCompRef;

};
