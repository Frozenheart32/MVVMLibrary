/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "PrimaryLayoutWidget.h"

UCommonActivatableWidgetContainerBase* UPrimaryLayoutWidget::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InTag), TEXT("UPrimaryLayoutWidget::FindWidgetStackByTag. Stack with tag %s not found!"), *InTag.ToString());

	return RegisteredWidgetStackMap[InTag];
}

void UPrimaryLayoutWidget::RegisterWidgetStack(FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if(!IsDesignTime())
	{
		if(InStack && InStackTag.IsValid() && !RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);
		}
	}
}
