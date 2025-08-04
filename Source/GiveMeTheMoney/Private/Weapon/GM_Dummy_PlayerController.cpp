// 무기 시연용 플레이어 컨트롤러
#include "Weapon/GM_Dummy_PlayerController.h"
#include "EnhancedInputSubsystems.h"

AGM_Dummy_PlayerController::AGM_Dummy_PlayerController()
	: InputMappingContext(nullptr), // 모든 입력 액션 초기화
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	ShotAction(nullptr),
	ReloadAction(nullptr),
	ChangeWeaponOneAction(nullptr),
	ChangeWeaponTwoAction(nullptr)
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


