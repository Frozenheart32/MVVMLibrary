/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "CommonMVVMHUD.generated.h"

class UPrimaryLayoutWidget;
class UUIView;

/**
 * 
 */
UCLASS()
class MVVMLIBRARY_API ACommonMVVMHUD : public AHUD
{
	GENERATED_BODY()

public:

	ACommonMVVMHUD();

private:

	UPROPERTY(EditDefaultsOnly, Category = "MVVM|HUD Settings")
	TSubclassOf<UPrimaryLayoutWidget> PrimaryLayoutWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "MVVM|HUD Settings")
	FGameplayTag StartupViewTag;

	UPROPERTY(EditDefaultsOnly, Category = "MVVM|HUD Settings")
	TArray<FGameplayTag> PreCachingViewTags;
	UPROPERTY(EditDefaultsOnly, Category = "MVVM|HUD Settings")
	TArray<FGameplayTag> PreCachingPopupTags;

protected:

	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void CreateAndRegisterPrimaryLayoutWidget();
	UFUNCTION()
	void StartPreCachingViewAndPopupTypes();
};
