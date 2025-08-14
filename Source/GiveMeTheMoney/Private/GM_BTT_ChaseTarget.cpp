#include "GM_BTT_ChaseTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UGM_BTT_ChaseTarget::UGM_BTT_ChaseTarget()
{
	NodeName = TEXT("Chase Target");
}

EBTNodeResult::Type UGM_BTT_ChaseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	AIController->MoveToActor(Target, 100.0f);
	return EBTNodeResult::Succeeded;
}
