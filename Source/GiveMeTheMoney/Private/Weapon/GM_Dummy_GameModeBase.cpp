// ���� �ÿ��� ���� ���

#include "Weapon/GM_Dummy_GameModeBase.h"
#include "Weapon/GM_Dummy_PlayerController.h"
#include "Weapon/Dummy_WeaponCharacter.h"

AGM_Dummy_GameModeBase::AGM_Dummy_GameModeBase()
{
	DefaultPawnClass = ADummy_WeaponCharacter::StaticClass();	// �⺻ ĳ���� ����
	PlayerControllerClass = AGM_Dummy_PlayerController::StaticClass();	// �÷��̾� ��Ʈ�ѷ� ����
}
