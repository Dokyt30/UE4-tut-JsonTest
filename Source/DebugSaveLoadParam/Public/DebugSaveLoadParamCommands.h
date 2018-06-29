// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "DebugSaveLoadParamStyle.h"

class FDebugSaveLoadParamCommands : public TCommands<FDebugSaveLoadParamCommands>
{
public:

	FDebugSaveLoadParamCommands()
		: TCommands<FDebugSaveLoadParamCommands>(TEXT("DebugSaveLoadParam"), NSLOCTEXT("Contexts", "DebugSaveLoadParam", "DebugSaveLoadParam Plugin"), NAME_None, FDebugSaveLoadParamStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};