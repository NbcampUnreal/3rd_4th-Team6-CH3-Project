// ���� �ÿ��� �÷��̾� ��Ʈ�ѷ�
#include "Weapon/GM_Dummy_PlayerController.h"
#include "EnhancedInputSubsystems.h"

AGM_Dummy_PlayerController::AGM_Dummy_PlayerController()
	: InputMappingContext(nullptr),        // �ش� �Է� �׼�, ���� ���� ���� nullptr�� �ʱ�ȭ
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

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())	// �÷��̾� ��Ʈ�ѷ��� ���� �÷��̾����� Ȯ��
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())	// Local Player�� EnhancedInputLocalPlayerSubsystem ��������
		{
			if (InputMappingContext)
			{
				SubSystem->AddMappingContext(InputMappingContext, 0);	// EnhancedInputLocalPlayerSubsystem�� InputMappingContext �߰�
			}
		}
	}
}


