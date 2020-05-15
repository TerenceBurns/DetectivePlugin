// Copyright 2020 - Trifolium Digital Limited

#include "QuickPluginConfigToolCommands.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"


void FQuickPluginConfigToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Detective Plugin", "Audit which plugins make it into your project.", EUserInterfaceActionType::Button, FInputGesture());
}


#undef LOCTEXT_NAMESPACE
