// Copyright 2020 - Trifolium Digital Limited

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "QuickPluginConfigToolStyle.h"

class FQuickPluginConfigToolCommands : public TCommands<FQuickPluginConfigToolCommands>
{
public:

	FQuickPluginConfigToolCommands()
		: TCommands<FQuickPluginConfigToolCommands>(TEXT("QuickPluginConfigTool"), NSLOCTEXT("Contexts", "QuickPluginConfigTool", "QuickPluginConfigTool Plugin"), NAME_None, FQuickPluginConfigToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};