// 무기 시연용 플레이어 컨트롤러
#include "Weapon/GM_Dummy_PlayerController.h"
#include "EnhancedInputSubsystems.h"

AGM_Dummy_PlayerController::AGM_Dummy_PlayerController()
	: InputMappingContext(nullptr),        // 해당 입력 액션, 매핑 값은 전부 nullptr로 초기화
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	FireAction(nullptr),
	CrouchAction(nullptr),
	EquipWeapon1Action(nullptr),
	EquipWeapon2Action(nullptr),
	ReloadAction(nullptr)
{
}

void AGM_Dummy_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())	// 플레이어 컨트롤러가 로컬 플레이어인지 확인
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())	// Local Player의 EnhancedInputLocalPlayerSubsystem 가져오기
		{
			if (InputMappingContext)
			{
				SubSystem->AddMappingContext(InputMappingContext, 0);	// EnhancedInputLocalPlayerSubsystem에 InputMappingContext 추가
			}
		}
	}
}


