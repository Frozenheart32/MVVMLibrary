/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "MVVMLibraryPluginSettings.generated.h"

class UUIPopupView;
class UUIView;

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta=(DisplayName = "MVVM Library Settings"))
class MVVMLIBRARY_API UMVVMLibraryPluginSettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:
	
	UPROPERTY(Config, EditAnywhere, Category = "View Reference", meta=(ForceInlineRow, Categories = "MVVM.View"))
	TMap<FGameplayTag, TSoftClassPtr<UUIView>> Views;

	UPROPERTY(Config, EditAnywhere, Category = "Popup Reference", meta=(ForceInlineRow, Categories = "MVVM.Popup"))
	TMap<FGameplayTag, TSoftClassPtr<UUIPopupView>> Popups;

public:

	UFUNCTION()
	TSoftClassPtr<UUIView> GetViewSoftClassByTag(const FGameplayTag& InViewTag) const;
	UFUNCTION()
	TSoftClassPtr<UUIPopupView> GetPopupSoftClassByTag(const FGameplayTag& InPopupTag) const;
};
