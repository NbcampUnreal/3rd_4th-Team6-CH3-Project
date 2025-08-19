#include "AI/GM_BTS_Detect.h"
#include "AI/GM_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "CoreMinimal.h"
#include "Character/GM_Character.h"

UGM_BTS_Detect::UGM_BTS_Detect()
{
    NodeName = TEXT("Detect");
    Interval = 1.0f;

    LostTargetDelay = 2.0f;  // Ÿ�� ��ģ �� ���� �ð�
    TimeSinceLastSeen = 0.0f;
}

void UGM_BTS_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* InPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (InPawn == nullptr) return;

    UWorld* World = InPawn->GetWorld();
    if (World == nullptr) return;

    FVector Center = InPawn->GetActorLocation();
    float DetectRadius = 600.0f;

    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams CollisionParams(NAME_None, false, InPawn);
    bool bResult = World->OverlapMultiByChannel(
        Overlaps,
        Center,
        FQuat::Identity,
        ECC_GameTraceChannel2,
        FCollisionShape::MakeSphere(DetectRadius),
        CollisionParams
    );

    bool bFoundTarget = false;
    AGM_Character* FoundTarget = nullptr;

    if (bResult)
    {
        for (auto const& Overlap : Overlaps)
        {
            AGM_Character* GMCharacter = Cast<AGM_Character>(Overlap.GetActor());
            if (GMCharacter && GMCharacter->GetController()->IsPlayerController())
            {
                bFoundTarget = true;
                FoundTarget = GMCharacter;
                break;
            }
        }
    }

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (bFoundTarget && FoundTarget)
    {
        // �þ� �ȿ� Ÿ�� ������ ������ Target ����
        Blackboard->SetValueAsObject(AGM_AIController::TargetKey, FoundTarget);
        TimeSinceLastSeen = 0.0f;

        // ����� ǥ��
        DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.1f);
        DrawDebugPoint(World, FoundTarget->GetActorLocation(), 10.0f, FColor::Blue, false, 0.1f);
        DrawDebugLine(World, InPawn->GetActorLocation(), FoundTarget->GetActorLocation(), FColor::Blue, false, 0.1f);
    }
    else
    {
        // Ÿ�� �� ���̸� Ÿ�̸� ����
        TimeSinceLastSeen += DeltaSeconds;
        if (TimeSinceLastSeen >= LostTargetDelay)
        {
            // ���� �ð� �̻� �� ���� Target ����
            Blackboard->SetValueAsObject(AGM_AIController::TargetKey, nullptr);
            TimeSinceLastSeen = 0.0f;

            DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
        }
    }
}
