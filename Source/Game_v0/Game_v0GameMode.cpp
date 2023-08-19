// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game_v0GameMode.h"
#include "Game_v0Character.h"
#include "UObject/ConstructorHelpers.h"

AGame_v0GameMode::AGame_v0GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
