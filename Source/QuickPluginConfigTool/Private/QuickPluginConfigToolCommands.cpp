// Copyright 2020 - Trifolium Digital Limited

#include "QuickPluginConfigToolCommands.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"

void FQuickPluginConfigToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "QuickPluginConfigTool", "Bring up QuickPluginConfigTool window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
