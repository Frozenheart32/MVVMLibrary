/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "Settings/MVVMLibraryPluginSettings.h"

TSoftClassPtr<UUIView> UMVVMLibraryPluginSettings::GetViewSoftClassByTag(const FGameplayTag& InViewTag) const
{
	checkf(InViewTag.IsValid(), TEXT("UMVVMDeveloperSettings::GetViewSoftClassByTag. InViewTag isn't valid"));
	checkf(Views.Contains(InViewTag),
		TEXT("UMVVMDeveloperSettings::GetViewSoftClassByTag. View with tag: %s not found! Check map, pls!"),
		*InViewTag.ToString());

	return Views[InViewTag];
}

TSoftClassPtr<UUIPopupView> UMVVMLibraryPluginSettings::GetPopupSoftClassByTag(const FGameplayTag& InPopupTag) const
{
	checkf(InPopupTag.IsValid(), TEXT("UMVVMDeveloperSettings::GetPopupSoftClassByTag. InViewTag isn't valid"));
	checkf(Popups.Contains(InPopupTag),
		TEXT("UMVVMDeveloperSettings::GetPopupSoftClassByTag. Popup with tag: %s not found! Check map, pls!"),
		*InPopupTag.ToString());

	return Popups[InPopupTag];
}
