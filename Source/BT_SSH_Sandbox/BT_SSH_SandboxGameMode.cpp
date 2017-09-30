// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BT_SSH_SandboxGameMode.h"
#include "BT_SSH_SandboxCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABT_SSH_SandboxGameMode::ABT_SSH_SandboxGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
