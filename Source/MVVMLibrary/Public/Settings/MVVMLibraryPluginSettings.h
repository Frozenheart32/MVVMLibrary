/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abstract/UIView.h"
#include "Abstract/UIPopUpView.h"
#include "Engine/DeveloperSettings.h"
#include "MVVMLibraryPluginSettings.generated.h"


/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta=(DisplayName = "MVVM Library Settings"))
class MVVMLIBRARY_API UMVVMLibraryPluginSettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:

	UPROPERTY(Config, EditAnywhere, Category = "Preload Types", meta=(ForceInlineRow, Categories = "MVVM.View"))
	TMap<FGameplayTag, TSubclassOf<UUIView>> PreloadedViews;
	UPROPERTY(Config, EditAnywhere, Category = "Preload Types", meta=(ForceInlineRow, Categories = "MVVM.View"))
	TMap<FGameplayTag, TSubclassOf<UUIPopupView>> PreloadedPopups;
	
	UPROPERTY(Config, EditAnywhere, Category = "View Reference", meta=(ForceInlineRow, Categories = "MVVM.View"))
	TMap<FGameplayTag, TSoftClassPtr<UUIView>> Views;

	UPROPERTY(Config, EditAnywhere, Category = "Popup Reference", meta=(ForceInlineRow, Categories = "MVVM.Popup"))
	TMap<FGameplayTag, TSoftClassPtr<UUIPopupView>> Popups;

public:

	UFUNCTION()
	TSoftClassPtr<UUIView> GetViewSoftClassByTag(const FGameplayTag& InViewTag) const;
	UFUNCTION()
	TSoftClassPtr<UUIPopupView> GetPopupSoftClassByTag(const FGameplayTag& InPopupTag) const;

	UFUNCTION()
	const TMap<FGameplayTag, TSubclassOf<UUIView>>& GetPreloadedViews() const;
	UFUNCTION()
	const TMap<FGameplayTag, TSubclassOf<UUIPopupView>>& GetPreloadedPopups() const;
	
};
