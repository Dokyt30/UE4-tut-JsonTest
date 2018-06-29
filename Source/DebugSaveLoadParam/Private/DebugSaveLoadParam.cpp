// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DebugSaveLoadParam.h"
#include "DebugSaveLoadParamStyle.h"
#include "DebugSaveLoadParamCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

static const FName DebugSaveLoadParamTabName("DebugSaveLoadParam");

#define LOCTEXT_NAMESPACE "FDebugSaveLoadParamModule"

void FDebugSaveLoadParamModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FDebugSaveLoadParamStyle::Initialize();
	FDebugSaveLoadParamStyle::ReloadTextures();

	FDebugSaveLoadParamCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FDebugSaveLoadParamCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FDebugSaveLoadParamModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FDebugSaveLoadParamModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FDebugSaveLoadParamModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(DebugSaveLoadParamTabName, FOnSpawnTab::CreateRaw(this, &FDebugSaveLoadParamModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FDebugSaveLoadParamTabTitle", "DebugSaveLoadParam"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FDebugSaveLoadParamModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FDebugSaveLoadParamStyle::Shutdown();

	FDebugSaveLoadParamCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(DebugSaveLoadParamTabName);
}

TSharedRef<SDockTab> FDebugSaveLoadParamModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FDebugSaveLoadParamModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("DebugSaveLoadParam.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FDebugSaveLoadParamModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(DebugSaveLoadParamTabName);
}

void FDebugSaveLoadParamModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FDebugSaveLoadParamCommands::Get().OpenPluginWindow);
}

void FDebugSaveLoadParamModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FDebugSaveLoadParamCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDebugSaveLoadParamModule, DebugSaveLoadParam)