// Copyright 2020 - Trifolium Digital Limited

#include "SPlatformFilter.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"


///////////////////////////////////////////////////////////////////////
// SPlatformFilterButton

/**
 * Slate widget which holds toggle for the given platform.
 */
class SPlatformFilterButton
	: public SCompoundWidget
{
public:
	DECLARE_DELEGATE_TwoParams(FOnFilterToggled, EPlatformFilter, bool);

	SLATE_BEGIN_ARGS(SPlatformFilterButton)
		: _PlatformStyleInfo()
		{}

		SLATE_DEFAULT_SLOT(FArguments, Content)
		SLATE_ARGUMENT(TSharedPtr<FPlatformStyleInfo>, PlatformStyleInfo)

		SLATE_EVENT(FOnFilterToggled, OnFilterToggled)
	SLATE_END_ARGS()


	/** Creation of the slate widget for the button. */
	void Construct(const FArguments& InArgs);

	/** */
	virtual ~SPlatformFilterButton() {}


private:

	/** 
	 * What should we do when the status of this platform is toggled?
	 */
	void OnCheckStatusChanged(ECheckBoxState EnabledCheckBoxState);

	/**
	 * What colour is the button?
	 */
	FSlateColor GetFilterForegroundColor() const;

private:

	// A handle for the current toggle status of this platform in filtering the view.
	TSharedPtr<SCheckBox> FilterCheckItem;

	// Information to help identify this platform in the view.
	TSharedPtr<FPlatformStyleInfo> PlatformStyleInfo;

	// We notify any listeners when this platform is toggled.
	FOnFilterToggled OnFilterToggled;
};


void SPlatformFilterButton::Construct(const FArguments& InArgs)
{
	PlatformStyleInfo = InArgs._PlatformStyleInfo;
	OnFilterToggled = InArgs._OnFilterToggled;

	FText PlatformLabel = PlatformStyleInfo->FilterId != EPlatformFilter::Editor_Only ? FText::FromString(PlatformStyleInfo->PlatformName) : LOCTEXT("EditorOnlyLabel", "Editor Only");

	ChildSlot
	[
		SAssignNew(FilterCheckItem, SCheckBox)
		.Style(FEditorStyle::Get(), "ContentBrowser.FilterButton")
		.OnCheckStateChanged(this, &SPlatformFilterButton::OnCheckStatusChanged)
		.ForegroundColor(this, &SPlatformFilterButton::GetFilterForegroundColor)
		.IsChecked(ECheckBoxState::Checked)
		[
			SNew(STextBlock)
			.Text(PlatformLabel)
		]
	];
}


void SPlatformFilterButton::OnCheckStatusChanged(ECheckBoxState EnabledCheckBoxState)
{
	OnFilterToggled.ExecuteIfBound(PlatformStyleInfo->FilterId, EnabledCheckBoxState == ECheckBoxState::Checked);
}


FSlateColor SPlatformFilterButton::GetFilterForegroundColor() const
{
	return FilterCheckItem->IsChecked() ? PlatformStyleInfo->StyleColour : FLinearColor::White;
}


///////////////////////////////////////////////////////////////////////
// SPlatformListFilter


void SPlatformListFilter::Construct(const FArguments& InArgs)
{
	OnPluginPlatformFilterChanged = InArgs._OnPluginPlatformFilterChanged;
	PlatformsExplicitlySupported = InArgs._PlatformsExplicitlySupported;
	
	FilterHBox = SNew(SHorizontalBox);

	for (auto& SupportedPlatform : PlatformStyleInfo::AllStyleInfo)
	{
		FilterHBox->AddSlot()
		.Padding(4.0f)
		.AutoWidth()
		[
			SNew(SPlatformFilterButton)
			.PlatformStyleInfo(SupportedPlatform)
			.OnFilterToggled(this, &SPlatformListFilter::OnFilterButtonToggled)
		];
	}
	
	// Editor Only Filter
	FilterHBox->AddSlot()
	.Padding(4.0f)
	.AutoWidth()
	[
		SNew(SPlatformFilterButton)
		.PlatformStyleInfo(MakeShareable(new FPlatformStyleInfo(TEXT("Editor Only"), PlatformColours::Editor_Only, EPlatformFilter::Editor_Only)))
		.OnFilterToggled(this, &SPlatformListFilter::OnFilterButtonToggled)
	];

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("PlatformFilterLabel", "Filter Plugins:"))
			.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
		]
		+ SVerticalBox::Slot()
		[
			FilterHBox.ToSharedRef()
		]
	];
}


SPlatformListFilter::~SPlatformListFilter()
{

}


void SPlatformListFilter::OnFilterButtonToggled(EPlatformFilter Filter, bool bWasEnabled)
{
	if (bWasEnabled)
		CurrentFilter |= Filter;
	else
		CurrentFilter = CurrentFilter & ~Filter;

	OnPluginPlatformFilterChanged.ExecuteIfBound(CurrentFilter);
}


#undef LOCTEXT_NAMESPACE