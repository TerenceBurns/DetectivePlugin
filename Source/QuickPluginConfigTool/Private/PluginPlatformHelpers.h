// Copyright 2020 - Trifolium Digital Limited

#pragma once

/** Platforms Supported
 * Win32
 * Win64
 * Mac
 * Linux
 * PS4
 * XboxOne
 * Switch
 * Android
 * IOS
 * TVOS
 * Lumin
 * HoloLens
 * 
 * etc.
 * 
 */

 /**
  * Identifiable colours for the different platforms.
  */
namespace PlatformColours
{
	static FColor Win64 = FColor(177, 70, 194);
	static FColor Win32 = FColor(177, 70, 194);

	static FColor Mac = FColor(0, 125, 221);
	static FColor Linux = FColor(255, 69, 0);
	static FColor LinuxAArch64 = FColor(255, 69, 0);

	static FColor PS4 = FColor(0, 55, 145);
	static FColor XboxOne = FColor(14, 122, 13);
	static FColor Switch = FColor(230, 0, 18);

	static FColor Android = FColor(13, 175, 84);
	static FColor IOS = FColor(0, 120, 215);
	static FColor TVOS = FColor(0, 120, 215);

	static FColor Lumin = FColor(241, 39, 66);
	static FColor HoloLens = FColor(140, 38, 122);

	static FColor Others = FColor(127, 127, 127);
	static FColor Editor_Only = FColor(255, 219, 88);

	static FColor All = FColor(75, 75, 75);
}

/**
 * Flags used to generate the list of plugins shown based on target platforms.
 */
enum class EPlatformFilter : uint32
{
	None =			0,
	Win64 =			1 << 0,
	Win32 =			1 << 1,

	Mac =			1 << 2,
	Linux =			1 << 3,
	LinuxAArch64 =	1 << 4,

	PS4 =			1 << 5,
	XboxOne =		1 << 6,
	Switch =		1 << 7,
	
	Android =		1 << 8,
	IOS =			1 << 9,
	TVOS =			1 << 10,

	Lumin =			1 << 11,
	HoloLens =		1 << 12,

	Others =		1 << 13,
	Editor_Only =	1 << 14,

	All = Win64 | Win32 | Mac | Linux | LinuxAArch64 | PS4 | XboxOne | Switch | Android | IOS | TVOS | Lumin | HoloLens | Others | Editor_Only
};
ENUM_CLASS_FLAGS(EPlatformFilter);


/**
 * Simple struct that holds some platform inforation for the plugin view.
 */
struct FPlatformStyleInfo
{
	FString PlatformName;
	FColor StyleColour;
	EPlatformFilter FilterId;

	FPlatformStyleInfo(FString&& InPlatformName, FColor InStyleColor, EPlatformFilter InFilterId)
		: PlatformName(MoveTemp(InPlatformName))
		, StyleColour(InStyleColor)
		, FilterId(InFilterId)
	{}
};


/**
 * Collection of specified platforms from stock UE4 plugins.
 * We use these to generate filters. Any platforms not in this list are covered by "Others"
 */
namespace PlatformStyleInfo
{
	static TSharedPtr<FPlatformStyleInfo> Win64StyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("Win64"), PlatformColours::Win64, EPlatformFilter::Win64));
	static TSharedPtr<FPlatformStyleInfo> Win32StyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("Win32"), PlatformColours::Win32, EPlatformFilter::Win32));
	static TSharedPtr<FPlatformStyleInfo> MacStyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("Mac"), PlatformColours::Mac, EPlatformFilter::Mac));
	static TSharedPtr<FPlatformStyleInfo> LinuxStyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("Linux"), PlatformColours::Linux, EPlatformFilter::Linux));
	static TSharedPtr<FPlatformStyleInfo> LinuxAArch64StyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("LinuxAArch64"), PlatformColours::LinuxAArch64, EPlatformFilter::LinuxAArch64));
	static TSharedPtr<FPlatformStyleInfo> PS4StyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("PS4"), PlatformColours::PS4, EPlatformFilter::PS4));
	static TSharedPtr<FPlatformStyleInfo> XboxOneStyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("XboxOne"), PlatformColours::XboxOne, EPlatformFilter::XboxOne));
	static TSharedPtr<FPlatformStyleInfo> SwitchStyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("Switch"), PlatformColours::Switch, EPlatformFilter::Switch));
	static TSharedPtr<FPlatformStyleInfo> AndroidStyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("Android"), PlatformColours::Android, EPlatformFilter::Android));
	static TSharedPtr<FPlatformStyleInfo> IOSStyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("IOS"), PlatformColours::IOS, EPlatformFilter::IOS));
	static TSharedPtr<FPlatformStyleInfo> TVOSStyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("TVOS"), PlatformColours::TVOS, EPlatformFilter::TVOS));
	static TSharedPtr<FPlatformStyleInfo> LuminStyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("Lumin"), PlatformColours::Lumin, EPlatformFilter::Lumin));
	static TSharedPtr<FPlatformStyleInfo> HoloLensStyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("HoloLens"), PlatformColours::HoloLens, EPlatformFilter::HoloLens));
	static TSharedPtr<FPlatformStyleInfo> OthersStyleInfo = MakeShareable(new FPlatformStyleInfo(TEXT("Others"), PlatformColours::Others, EPlatformFilter::Others));

	static TArray<TSharedPtr<FPlatformStyleInfo>> AllStyleInfo =
	{
		Win64StyleInfo,
		Win32StyleInfo,
		MacStyleInfo,
		LinuxStyleInfo,
		LinuxAArch64StyleInfo,
		PS4StyleInfo,
		XboxOneStyleInfo,
		SwitchStyleInfo,
		AndroidStyleInfo,
		IOSStyleInfo,
		TVOSStyleInfo,
		LuminStyleInfo,
		HoloLensStyleInfo,
		OthersStyleInfo
	};
}


/**
 * Simple conversion of platform name to platform colour specified by us above.
 */
FColor GetBorderColourForPlatform(const FString& InPlatformName)
{
	if (InPlatformName == TEXT("Win64"))
		return PlatformColours::Win64;
	else if (InPlatformName == TEXT("Win32"))
		return PlatformColours::Win32;
	else if (InPlatformName == TEXT("Mac"))
		return PlatformColours::Mac;
	else if (InPlatformName == TEXT("Linux"))
		return PlatformColours::Linux;
	else if (InPlatformName == TEXT("LinuxAArch64"))
		return PlatformColours::LinuxAArch64;
	else if (InPlatformName == TEXT("PS4"))
		return PlatformColours::PS4;
	else if (InPlatformName == TEXT("XboxOne"))
		return PlatformColours::XboxOne;
	else if (InPlatformName == TEXT("Switch"))
		return PlatformColours::Switch;
	else if (InPlatformName == TEXT("Android"))
		return PlatformColours::Android;
	else if (InPlatformName == TEXT("IOS"))
		return PlatformColours::IOS;
	else if (InPlatformName == TEXT("TVOS"))
		return PlatformColours::TVOS;
	else if (InPlatformName == TEXT("Lumin"))
		return PlatformColours::Lumin;
	else if (InPlatformName == TEXT("HoloLens"))
		return PlatformColours::HoloLens;
	else
		return PlatformColours::Others;
}


/**
 * Parse filter flags for list of platforms (in string format).
 */
TArray<FString> PlatformFilterToPlatformIdStrings(EPlatformFilter NewFilter)
{
	TArray<FString> PlatformIdStrings;
	if (!!(NewFilter & EPlatformFilter::Win64))
		PlatformIdStrings.Add(TEXT("Win64"));
	if (!!(NewFilter & EPlatformFilter::Win32))
		PlatformIdStrings.Add(TEXT("Win32"));
	if (!!(NewFilter & EPlatformFilter::Mac))
		PlatformIdStrings.Add(TEXT("Mac"));
	if (!!(NewFilter & EPlatformFilter::Linux))
		PlatformIdStrings.Add(TEXT("Linux"));
	if (!!(NewFilter & EPlatformFilter::LinuxAArch64))
		PlatformIdStrings.Add(TEXT("LinuxAArch64"));
	if (!!(NewFilter & EPlatformFilter::PS4))
		PlatformIdStrings.Add(TEXT("PS4"));
	if (!!(NewFilter & EPlatformFilter::XboxOne))
		PlatformIdStrings.Add(TEXT("XboxOne"));
	if (!!(NewFilter & EPlatformFilter::Switch))
		PlatformIdStrings.Add(TEXT("Switch"));
	if (!!(NewFilter & EPlatformFilter::Android))
		PlatformIdStrings.Add(TEXT("Android"));
	if (!!(NewFilter & EPlatformFilter::IOS))
		PlatformIdStrings.Add(TEXT("IOS"));
	if (!!(NewFilter & EPlatformFilter::TVOS))
		PlatformIdStrings.Add(TEXT("TVOS"));
	if (!!(NewFilter & EPlatformFilter::Lumin))
		PlatformIdStrings.Add(TEXT("Lumin"));
	if (!!(NewFilter & EPlatformFilter::HoloLens))
		PlatformIdStrings.Add(TEXT("HoloLens"));
	if (!!(NewFilter & EPlatformFilter::Others))
		PlatformIdStrings.Add(TEXT("Others"));

	return PlatformIdStrings;
}


/**
 * Convert platform string to EPlatformFilter id.
 */
EPlatformFilter PlatformIdStringsToFilterId(const FString InPlatformId)
{
	if (InPlatformId == TEXT("Win64"))
		return EPlatformFilter::Win64;
	else if(InPlatformId == TEXT("Win32"))
		return EPlatformFilter::Win32;
	else if(InPlatformId == TEXT("Mac"))
		return EPlatformFilter::Mac;
	else if(InPlatformId == TEXT("Linux"))
		return EPlatformFilter::Linux;
	else if (InPlatformId == TEXT("LinuxAArch64"))
		return EPlatformFilter::LinuxAArch64;
	else if(InPlatformId == TEXT("PS4"))
		return EPlatformFilter::PS4;
	else if(InPlatformId == TEXT("XboxOne"))
		return EPlatformFilter::XboxOne;
	else if(InPlatformId == TEXT("Switch"))
		return EPlatformFilter::Switch;
	else if(InPlatformId == TEXT("Android"))
		return EPlatformFilter::Android;
	else if(InPlatformId == TEXT("IOS"))
		return EPlatformFilter::IOS;
	else if(InPlatformId == TEXT("TVOS"))
		return EPlatformFilter::TVOS;
	else if(InPlatformId == TEXT("Lumin"))
		return EPlatformFilter::Lumin;
	else if(InPlatformId == TEXT("HoloLens"))
		return EPlatformFilter::HoloLens;
	else
		return EPlatformFilter::Others;
}