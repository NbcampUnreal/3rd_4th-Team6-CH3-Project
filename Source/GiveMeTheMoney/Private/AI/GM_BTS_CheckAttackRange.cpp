#include "AI/GM_BTS_CheckAttackRange.h"
#include "AI/GM_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UGM_BTS_CheckAttackRange::UGM_BTS_CheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");
}

void UGM_BTS_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!CurrentPawn)
	{
		return;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject("Target"));
	if (!Target)
	{
		BlackboardComp->SetValueAsBool("InAttackRange", false);
		return;
	}

	float Dist = FVector::Dist(CurrentPawn->GetActorLocation(), Target->GetActorLocation());
	BlackboardComp->SetValueAsBool("InAttackRange", Dist <= AttackRange);
}