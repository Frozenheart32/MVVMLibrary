/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abstract/UIView.h"
#include "Subsystems/WorldSubsystem.h"
#include "WindowSubsystem.generated.h"

class UPrimaryLayoutWidget;
class UUIPopupView;
class UWorldModelRepositorySubsystem;
class APlayerController;
class UModelRepositorySubsystem;
class UUIView;
class UPanelWidget;

UENUM(BlueprintType)
enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush,
};

/**
 * Serves for spawning, storing and closing windows. Life cycle is one scene
 */
UCLASS(NotBlueprintable, BlueprintType)
class MVVMLIBRARY_API UWindowSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TObjectPtr<UPrimaryLayoutWidget> PrimaryLayoutWidget = nullptr;

	UPROPERTY()
	TMap<FGameplayTag, TSubclassOf<UUIView>> CachedViewTypes;
	UPROPERTY()
	TMap<FGameplayTag, TSubclassOf<UUIPopupView>> CachedPopupTypes;

	UPROPERTY()
	mutable TWeakObjectPtr<UWorldModelRepositorySubsystem> CachedWorldModelRepository = nullptr;
	UPROPERTY()
	mutable TWeakObjectPtr<UModelRepositorySubsystem> CachedModelRepository = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "MVVM|WindowSubsystem", meta=(AllowPrivateAccess))
	bool bIsHiddenAllWindows = false;

protected:

	UFUNCTION(BlueprintCallable, DisplayName = "PushViewToStackByTag", Category = "MVVM|WindowSubsystem")
	void K2_PushViewToStackByTagAsync(APlayerController* OwningPlayerController, UPARAM(meta = (Categories = "MVVMLayout.WidgetStack"))FGameplayTag InWidgetStackTag, UPARAM(meta = (Categories = "MVVM.View"))FGameplayTag InViewTag, bool bIsNeedAddTypeToCache = true, bool bFocusOnNewlyPushedWidget = true);
	UFUNCTION(BlueprintCallable, DisplayName = "PushViewToStackBySoftClass", Category = "MVVM|WindowSubsystem")
	void K2_PushViewToStackBySoftClassAsync(APlayerController* OwningPlayerController, UPARAM(meta = (Categories = "MVVMLayout.WidgetStack"))FGameplayTag InWidgetStackTag, TSoftClassPtr<UUIView> ViewSoftType, bool bFocusOnNewlyPushedWidget = true);

	UFUNCTION(BlueprintCallable, DisplayName = "PushPopupToStackByTag", Category = "MVVM|WindowSubsystem")
	void K2_PushPopupToStackByTagAsync(APlayerController* OwningPlayerController, UPARAM(meta = (Categories = "MVVMLayout.WidgetStack"))FGameplayTag InWidgetStackTag, UPARAM(meta = (Categories = "MVVM.Popup"))FGameplayTag InPopupTag, bool bIsNeedAddTypeToCache = true, bool bFocusOnNewlyPushedWidget = true, UObject* InFeedDataObject = nullptr);
	UFUNCTION(BlueprintCallable, DisplayName = "PushPopupToStackBySoftClass", Category = "MVVM|WindowSubsystem")
	void K2_PushPopupToStackBySoftClassAsync(APlayerController* OwningPlayerController, UPARAM(meta = (Categories = "MVVMLayout.WidgetStack"))FGameplayTag InWidgetStackTag, TSoftClassPtr<UUIPopupView> PopupSoftType, bool bFocusOnNewlyPushedWidget = true, UObject* InFeedDataObject = nullptr);
	

public:

	static UWindowSubsystem* Get(const UObject* WorldContextObject);
	
	/**
	 * Creating and register Primary Layout Widget. By default, uses to ACommonMVVMHUD logic.
	 * @param OwningPlayerController
	 * @param InLayoutWidgetClass
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	void CreateAndRegisterCreatedPrimaryLayoutWidget(APlayerController* OwningPlayerController, const TSubclassOf<UPrimaryLayoutWidget>& InLayoutWidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	void StartPreCachingViewTypes(const TArray<FGameplayTag>& InCachingViewTags);
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	void StartPreCachingPopupTypes(const TArray<FGameplayTag>& InCachingPopupTags);
	
	void PushViewToStackByTagAsync(APlayerController* OwningPlayerController, const FGameplayTag& InWidgetStackTag, const FGameplayTag& InViewTag, bool bIsNeedAddTypeToCache, bool bFocusOnNewlyPushedWidget = true, TFunction<void(EAsyncPushWidgetState, UUIView*)> AsyncPushStateCallback = {});
	void PushViewToStackBySoftClassAsync(APlayerController* OwningPlayerController, const FGameplayTag& InWidgetStackTag, const TSoftClassPtr<UUIView>& ViewSoftType, bool bFocusOnNewlyPushedWidget = true, TFunction<void(EAsyncPushWidgetState, UUIView*)> AsyncPushStateCallback = {});
	
	void PushPopupToStackByTagAsync(APlayerController* OwningPlayerController, const FGameplayTag& InWidgetStackTag, const FGameplayTag& InPopupTag, bool bIsNeedAddTypeToCache, bool bFocusOnNewlyPushedWidget = true, UObject* InFeedDataObject = nullptr, TFunction<void(EAsyncPushWidgetState, UUIPopupView*)> AsyncPushStateCallback = {});
	void PushPopupToStackBySoftClassAsync(APlayerController* OwningPlayerController, const FGameplayTag& InWidgetStackTag, const TSoftClassPtr<UUIPopupView>& PopupSoftType, bool bFocusOnNewlyPushedWidget = true, UObject* InFeedDataObject = nullptr, TFunction<void(EAsyncPushWidgetState, UUIPopupView*)> AsyncPushStateCallback = {});
	

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	void InitializeExistsView(UUIView* ExistedView);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	void HideAllWindows();
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	void ShowAllWindows();

private:

	void CreateView(APlayerController* OwningPlayerController, const TSubclassOf<UUIView>& LoadedViewClass, const FGameplayTag& InWidgetStackTag, bool bFocusOnNewlyPushedWidget, TFunction<void(EAsyncPushWidgetState, UUIView*)> AsyncPushStateCallback);
	void CreatePopup(APlayerController* OwningPlayerController, const TSubclassOf<UUIPopupView>& LoadedPopupClass, const FGameplayTag& InWidgetStackTag, bool bFocusOnNewlyPushedWidget, UObject* InFeedDataObject, TFunction<void(EAsyncPushWidgetState, UUIPopupView*)> AsyncPushStateCallback);

	UFUNCTION()
	UModelRepositorySubsystem* GetModelRepositorySubsystem() const;
	UFUNCTION()
	UWorldModelRepositorySubsystem* GetWorldModelRepositorySubsystem() const;
};
