#include "GM_AIController.h"

#include "GM_AICharacter.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AGM_AIController::HomePosKey(TEXT("HomePos"));
const FName AGM_AIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AGM_AIController::TargetKey(TEXT("Target"));

AGM_AIController::AGM_AIController(): RepeatInterval(0)
{
	//static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(
	//	TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_Mainboard.BB_Mainboard'"));
	//if (BD.Succeeded())
	//{
	//	BBAsset = BD.Object;
	//}
//
	//static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(
	//	TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_MainTree.BT_MainTree'"));
	//if (BT.Succeeded())
	//{
	//	BTAsset = BT.Object;
	//}
}

void AGM_AIController::BeginPlay()
{
	Super::BeginPlay();

	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AGM_AIController::GMCHECK(bool bCondition)
{
	if (!bCondition)
	{
		UE_LOG(LogTemp, Warning, TEXT("GMCHECK failed!"));
	}
}

void AGM_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AGM_AIController::OnRepeatTimer, RepeatInterval, true);

	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		BlackboardComp->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("RunBehaviorTree failed"));
		}
	}
}

void AGM_AIController::OnUnPossess()
{
	Super::OnUnPossess();
	//GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AGM_AIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	GMCHECK(nullptr != CurrentPawn);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr) return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 600.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		UE_LOG (LogTemp, Warning, TEXT("Next Location: %s"), *NextLocation.Location.ToString());
	}
}

