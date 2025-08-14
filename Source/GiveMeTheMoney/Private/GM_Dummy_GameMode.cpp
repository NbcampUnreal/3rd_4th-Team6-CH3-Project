// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Dummy_GameMode.h"
#include "GM_Character.h"
#include "GM_Controller_Character.h"

AGM_Dummy_GameMode::AGM_Dummy_GameMode()
{
	DefaultPawnClass = AGM_Character::StaticClass();

	PlayerControllerClass = AGM_Controller_Character::StaticClass();

}