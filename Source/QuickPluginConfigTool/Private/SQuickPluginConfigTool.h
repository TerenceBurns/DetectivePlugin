// Copyright 2020 - Trifolium Digital Limited

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"


/**
 * Slate widget to allow users to edit which Plugins are enabled for this project.
 */
class SQuickPluginConfigTool
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuickPluginConfigTool) {}
		SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()


	/** The body of the slate window for our plugin settings widget. */
	void Construct(const FArguments& InArgs);

	/** Destructor */
	virtual ~SQuickPluginConfigTool();

// Begin SWidget interface
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
// End SWidget interface

private:
	// Reference to the table view which displays information about the filtered plugins. 
	TSharedPtr<class SQuickPluginListView> PluginDetailsView;

	// Reference to the slate widget which shows us relevant information about the status of the .uproject file.
	TSharedPtr<class SProjectFileInfo> ProjectFileInfo;

	// Was the .uproject file readonly or writable the last time we checked.
	bool bLastProjectCheckWasReadOnly = false;

	// When was the last time we determined the status of the .uproject file?
	double LastReadOnlyCheckTime = 0.0;
};