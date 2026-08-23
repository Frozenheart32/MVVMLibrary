/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "PrimaryLayoutWidget.generated.h"

class UCommonActivatableWidgetContainerBase;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, meta=(DisableNativeTick))
class MVVMLIBRARY_API UPrimaryLayoutWidget : public UCommonUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;

public:

	UFUNCTION(BlueprintCallable, Category = "MVVM|Primary Layout Widget")
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InTag) const;
	
protected:

	UFUNCTION(BlueprintCallable, meta=(BlueprintProtected), Category = "MVVM|Primary Layout Widget")
	void RegisterWidgetStack(UPARAM(meta=(Categories = "MVVMLayout.WidgetStack"))FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack);
	
};
