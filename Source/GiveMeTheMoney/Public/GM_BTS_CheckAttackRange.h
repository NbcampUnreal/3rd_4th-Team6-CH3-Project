#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GM_BTS_CheckAttackRange.generated.h"

UCLASS()
class GIVEMETHEMONEY_API UGM_BTS_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
public:
	UGM_BTS_CheckAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 200.0f;

};
