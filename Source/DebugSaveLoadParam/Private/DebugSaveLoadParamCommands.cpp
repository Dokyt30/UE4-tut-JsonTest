// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DebugSaveLoadParamCommands.h"

#define LOCTEXT_NAMESPACE "FDebugSaveLoadParamModule"

void FDebugSaveLoadParamCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "DebugSaveLoadParam", "Bring up DebugSaveLoadParam window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
