// Copyright 2020 - Trifolium Digital Limited

#include "QuickPluginConfigTool.h"
#include "QuickPluginConfigToolStyle.h"
#include "QuickPluginConfigToolCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "SQuickPluginConfigTool.h"

static const FName QuickPluginConfigToolTabName("QuickPluginConfigTool");


#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"


void FQuickPluginConfigToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FQuickPluginConfigToolStyle::Initialize();
	FQuickPluginConfigToolStyle::ReloadTextures();

	FQuickPluginConfigToolCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FQuickPluginConfigToolCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FQuickPluginConfigToolModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FQuickPluginConfigToolModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FQuickPluginConfigToolModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(QuickPluginConfigToolTabName, FOnSpawnTab::CreateRaw(this, &FQuickPluginConfigToolModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FQuickPluginConfigToolTabTitle", "Detective Plugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}


void FQuickPluginConfigToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FQuickPluginConfigToolStyle::Shutdown();

	FQuickPluginConfigToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(QuickPluginConfigToolTabName);
}


TSharedRef<SDockTab> FQuickPluginConfigToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SQuickPluginConfigTool)
		];
}


void FQuickPluginConfigToolModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(QuickPluginConfigToolTabName);
}


void FQuickPluginConfigToolModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FQuickPluginConfigToolCommands::Get().OpenPluginWindow);
}


void FQuickPluginConfigToolModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FQuickPluginConfigToolCommands::Get().OpenPluginWindow);
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuickPluginConfigToolModule, QuickPluginConfigTool)