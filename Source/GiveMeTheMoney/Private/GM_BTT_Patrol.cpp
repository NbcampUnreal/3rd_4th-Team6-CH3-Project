#include "GM_BTT_Patrol.h"
#include "AIController.h"
#include "GM_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UGM_BTT_Patrol::UGM_BTT_Patrol()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UGM_BTT_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(CurrentPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AGM_AIController::HomePosKey);
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		UE_LOG(LogTemp, Warning, TEXT("Next Patrol Location: %s"), *NextPatrol.Location.ToString());

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AGM_AIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;

		
	}
	return EBTNodeResult::Failed;
}
