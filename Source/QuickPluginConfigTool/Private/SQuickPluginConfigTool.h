// Copyright 2020 - Trifolium Digital Limited

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"

/**
 * Slate widget to allow users to edit Config Settings
 */
class SQuickPluginConfigTool
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuickPluginConfigTool) {}
		SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()


	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/** Destructor */
	~SQuickPluginConfigTool();

private:
	/** Holds the vertical box for ini file categories. */
	TSharedPtr<class SQuickPluginListView> PluginDetailsView;
};