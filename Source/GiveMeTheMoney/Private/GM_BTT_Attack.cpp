#include "GM_BTT_Attack.h"
#include "AIController.h"
#include "GM_AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UGM_BTT_Attack::UGM_BTT_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UGM_BTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto AICharacter = Cast<AGM_AICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == AICharacter)
	{
		return EBTNodeResult::Failed;
	}

	OwnerCompRef = &OwnerComp;
	AICharacter->Attack();
	IsAttacking = true;

	AICharacter->OnAttackEnd.AddDynamic(this, &UGM_BTT_Attack::OnAttackEnded);

	return EBTNodeResult::InProgress;
}

void UGM_BTT_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UGM_BTT_Attack::OnAttackEnded()
{
	FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
}
