/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushPopupByTag.generated.h"

class UUIPopupView;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushPopupByTagDelegate, UUIPopupView*, PushedPopup);

/**
 * 
 */
UCLASS()
class MVVMLIBRARY_API UAsyncAction_PushPopupByTag : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnPushPopupByTagDelegate CreatedBeforePush;
	UPROPERTY(BlueprintAssignable)
	FOnPushPopupByTagDelegate AfterPush;

	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Popup By Tag Async Action"))
	static UAsyncAction_PushPopupByTag* PushPopupByTag(const UObject* WorldContextObject, APlayerController* PlayerController,
		UPARAM(meta = (Categories = "MVVMLayout.WidgetStack"))FGameplayTag WidgetStackTag,
		UPARAM(meta = (Categories = "MVVM.Popup"))FGameplayTag PopupTag,
		UObject* FeedDataObject = nullptr,
		bool bIsNeedAddViewTypeToCache = true,
		bool bFocusOnNewlyPushedWidget = true);

	virtual void Activate() override;

private:

	UPROPERTY()
	TWeakObjectPtr<UWorld> CachedOwningWorld = nullptr;
	UPROPERTY()
	TWeakObjectPtr<APlayerController> CachedOwningPC = nullptr;
	UPROPERTY()
	FGameplayTag CachedWidgetStackTag = FGameplayTag{};
	UPROPERTY()
	FGameplayTag CachedPopupTag = FGameplayTag{};
	UPROPERTY()
	TObjectPtr<UObject> CachedFeedDataObject = nullptr;
	UPROPERTY()
	bool bCachedIsNeedAddViewTypeToCache = false;
	UPROPERTY()
	bool bCachedFocusOnNewlyPushedWidget = true;


	UFUNCTION()
	void FinishAction();
};
