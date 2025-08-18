// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GM_WeaponDummy_GameMode.h"
#include "Weapon/GM_Dummy_PlayerController.h"
#include "Weapon/Dummy_WeaponCharacter.h"

AGM_WeaponDummy_GameMode::AGM_WeaponDummy_GameMode()
{
	DefaultPawnClass = ADummy_WeaponCharacter::StaticClass();	// �⺻ ĳ���� ����
	PlayerControllerClass = AGM_Dummy_PlayerController::StaticClass();	// �÷��̾� ��Ʈ�ѷ� ����
}
