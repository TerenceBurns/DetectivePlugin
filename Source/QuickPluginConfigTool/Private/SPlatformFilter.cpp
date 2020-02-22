// Copyright 2020 - Trifolium Digital Limited

#include "SPlatformFilter.h"

#define LOCTEXT_NAMESPACE "FQuickPluginConfigToolModule"


///////////////////////////////////////////////////////////////////////
// SPlatformListFilter

class SPlatformFilterButton
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPlatformFilterButton) 
		: _PlatformStyleInfo()
		{}

		SLATE_DEFAULT_SLOT(FArguments, Content)
		SLATE_ATTRIBUTE(FPlatformStyleInfo, PlatformStyleInfo)
	SLATE_END_ARGS()


	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs)
	{
		PlatformStyleInfo = InArgs._PlatformStyleInfo;
		ChildSlot
		[
			SAssignNew(FilterCheckItem, SCheckBox)
			.Style(FEditorStyle::Get(), "ContentBrowser.FilterButton")
			.OnCheckStateChanged(this, &SPlatformFilterButton::OnCheckStatusChanged)
			.ForegroundColor(this, &SPlatformFilterButton::GetFilterForegroundColor)
			.IsChecked(ECheckBoxState::Checked)
			[
				SNew(STextBlock)
				.Text(FText::FromString(PlatformStyleInfo.Get().PlatformName))
			]
		];
	}

	/** Destructor */
	~SPlatformFilterButton() {}


private:

	/** 
	 *
	 */
	void OnCheckStatusChanged(ECheckBoxState EnabledCheckBoxState)
	{
	}

	/**
	 *
	 */
	FSlateColor GetFilterForegroundColor() const
	{
		return FilterCheckItem->IsChecked() ? PlatformStyleInfo.Get().StyleColour : FLinearColor::White;
	}

private:

	//
	TSharedPtr<SCheckBox> FilterCheckItem;

	//
	TAttribute<FPlatformStyleInfo> PlatformStyleInfo;
};


///////////////////////////////////////////////////////////////////////
// SPlatformListFilter


void SPlatformListFilter::Construct(const FArguments& InArgs)
{
	FilterHBox = SNew(SHorizontalBox);

	FPlatformStyleInfo Test;
	Test.PlatformName = TEXT("PlayBox1");

	for (int32 i = 0; i < 5; i++)
	{
		Test.StyleColour = FColor(127, 30*i, 57);
		FilterHBox->AddSlot()
		.Padding(4.0f)
		.AutoWidth()
		[
			SNew(SPlatformFilterButton)
			.PlatformStyleInfo(Test)
		];
	}

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			FilterHBox.ToSharedRef()
		]
	];
}


SPlatformListFilter::~SPlatformListFilter()
{

}

#undef LOCTEXT_NAMESPACE