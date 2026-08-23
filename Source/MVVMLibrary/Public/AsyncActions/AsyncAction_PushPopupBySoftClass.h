/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushPopupBySoftClass.generated.h"

class UUIPopupView;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushPopupBySoftClassDelegate, UUIPopupView*, PushedPopup);

/**
 * 
 */
UCLASS()
class MVVMLIBRARY_API UAsyncAction_PushPopupBySoftClass : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:

	UPROPERTY(BlueprintAssignable)
	FOnPushPopupBySoftClassDelegate CreatedBeforePush;
	UPROPERTY(BlueprintAssignable)
	FOnPushPopupBySoftClassDelegate AfterPush;

	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Popup By SoftClass Async Action"))
	static UAsyncAction_PushPopupBySoftClass* PushViewByTag(const UObject* WorldContextObject, APlayerController* PlayerController,
		UPARAM(meta = (Categories = "MVVMLayout.WidgetStack"))FGameplayTag WidgetStackTag,
		TSoftClassPtr<UUIPopupView> PopupSoftClass, UObject* FeedDataObject = nullptr,
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
	TSoftClassPtr<UUIPopupView> CachedPopupSoftType;
	UPROPERTY()
	TObjectPtr<UObject> CachedFeedDataObject = nullptr;
	UPROPERTY()
	bool bCachedFocusOnNewlyPushedWidget = true;

	UFUNCTION()
	void FinishAction();
};
