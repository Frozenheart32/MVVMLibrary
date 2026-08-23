/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MVVMPluginFunctionLibrary.generated.h"

class UUIPopupView;
class UUIView;

/**
 * 
 */
UCLASS()
class MVVMLIBRARY_API UMVVMPluginFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MVVM Plugin Function Library")
	static TSoftClassPtr<UUIView> GetViewSoftClassByTag(FGameplayTag InViewTag);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MVVM Plugin Function Library")
	static TSoftClassPtr<UUIPopupView> GetPopupSoftClassByTag(FGameplayTag InPopupTag);
};
