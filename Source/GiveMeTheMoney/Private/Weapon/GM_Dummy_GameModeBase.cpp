// 무기 시연용 게임 모드

#include "Weapon/GM_Dummy_GameModeBase.h"
#include "Weapon/GM_Dummy_PlayerController.h"
#include "Weapon/Dummy_WeaponCharacter.h"

AGM_Dummy_GameModeBase::AGM_Dummy_GameModeBase()
{
	DefaultPawnClass = ADummy_WeaponCharacter::StaticClass();	// 기본 캐릭터 설정
	PlayerControllerClass = AGM_Dummy_PlayerController::StaticClass();	// 플레이어 컨트롤러 설정
}
