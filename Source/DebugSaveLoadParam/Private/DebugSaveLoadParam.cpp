// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DebugSaveLoadParam.h"
#include "DebugSaveLoadParamStyle.h"
#include "DebugSaveLoadParamCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SVectorInputBox.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include <string>
#include <fstream>
#include "Serialization/JsonWriter.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Json/Public/Serialization/JsonReader.h"

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
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				// 文字列入力欄
				// ↓ SAssignNewに置き換え！
				SAssignNew(InputBoxWidget, SEditableTextBox)
				.HintText(LOCTEXT("HintText", "名前を入れてください"))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					// 文字列入力欄
					// ↓ SAssignNewに置き換え！
					SNew(SButton)
					.Text(LOCTEXT("LoadButtonText", "Load"))
					.OnClicked_Raw(this, &FDebugSaveLoadParamModule::OnClickedLoadButton)
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("SaveButtonText", "Save"))
					.OnClicked_Raw(this, &FDebugSaveLoadParamModule::OnClickedSaveButton)
				]
			]
		];
}

void FDebugSaveLoadParamModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(DebugSaveLoadParamTabName);
}
FReply FDebugSaveLoadParamModule::OnClickedLoadButton()
{
	UE_LOG(LogTemp, Log, TEXT("Load"));
	{
		FString filepath_str = FPaths::ProjectLogDir() / "DebugSaveData.json";
		const std::string filepath(TCHAR_TO_UTF8(*filepath_str));
		if (InputBoxWidget.IsValid()) {
			const FText InputText = InputBoxWidget->GetText();
			FString _tmpStr = InputText.ToString();
			const std::string edittext(TCHAR_TO_UTF8(*_tmpStr));


			FString JsonStr;
			FFileHelper::LoadFileToString(JsonStr, *filepath_str);
			// JsonReader! ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonStr);
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());


			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				FString recievedInt = JsonObject->GetStringField(TEXT("name"));
				const FText InputText = FText::FromString(recievedInt);
				InputBoxWidget->SetText(InputText);
			}


			//std::ifstream  itr(filepath, std::ios::app);
			//std::string str;
			//if (itr.fail())
			//{
			//	//std::cerr << "失敗" << std::endl;
			//	return FReply::Handled();
			//}
			//while (getline(itr, str))
			//{
			//	FString _tempStr = FString(UTF8_TO_TCHAR(str.c_str()));
			//	const FText InputText = FText::FromString(_tempStr);
			//	InputBoxWidget->SetText(InputText);
			//	//std::cout << "[" << str << "]" << std::endl;
			//}

		}
	}


	return FReply::Handled();
}
FReply FDebugSaveLoadParamModule::OnClickedSaveButton()
{
	UE_LOG(LogTemp, Log, TEXT("Save"));

	{
		FString filepath_str = FPaths::ProjectLogDir() / "DebugSaveData.json";
		FString JsonStr;
		const std::string filepath(TCHAR_TO_UTF8(*filepath_str));
		if (InputBoxWidget.IsValid()) {
			const FText InputText = InputBoxWidget->GetText();
			FString _tmpStr = InputText.ToString();
			const std::string edittext(TCHAR_TO_UTF8(*_tmpStr));

			std::ofstream ostr(filepath, std::ios::app);
			ostr << edittext << std::endl;
		}
	}
	return FReply::Handled();
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