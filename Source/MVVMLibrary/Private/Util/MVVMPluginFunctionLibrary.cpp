/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "Util/MVVMPluginFunctionLibrary.h"

#include "Settings/MVVMLibraryPluginSettings.h"

TSoftClassPtr<UUIView> UMVVMPluginFunctionLibrary::GetViewSoftClassByTag(FGameplayTag InViewTag)
{
	const auto MVVMSettings = GetDefault<UMVVMLibraryPluginSettings>();
	check(MVVMSettings);

	return MVVMSettings->GetViewSoftClassByTag(InViewTag);
}

TSoftClassPtr<UUIPopupView> UMVVMPluginFunctionLibrary::GetPopupSoftClassByTag(FGameplayTag InPopupTag)
{
	const auto MVVMSettings = GetDefault<UMVVMLibraryPluginSettings>();
	check(MVVMSettings);

	return MVVMSettings->GetPopupSoftClassByTag(InPopupTag);
}
