/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"

#include "NativeGameplayTags.h"


namespace MVVMLayoutTags
{
	//widget stack
	MVVMLIBRARY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MVVMLayout_WidgetStack_BeforeHud);
	MVVMLIBRARY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MVVMLayout_WidgetStack_GameHud);
	MVVMLIBRARY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MVVMLayout_WidgetStack_GameMenu);
	MVVMLIBRARY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MVVMLayout_WidgetStack_PopUp);
}
