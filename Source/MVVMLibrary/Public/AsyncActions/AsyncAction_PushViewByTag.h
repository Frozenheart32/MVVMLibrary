/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushViewByTag.generated.h"


class UUIView;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushViewByTagDelegate, UUIView*, PushedView);

/**
* Used for push views. Search for the view's softclass by tag (in EditSettings -> MVVM plugin settings -> ViewMap variable).
* You can add the view type to the cache to instantly invoke the desired window next time (for frequently opened views).
 */
UCLASS()
class MVVMLIBRARY_API UAsyncAction_PushViewByTag : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnPushViewByTagDelegate CreatedBeforePush;
	UPROPERTY(BlueprintAssignable)
	FOnPushViewByTagDelegate AfterPush;

	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push View By Tag Async Action"))
	static UAsyncAction_PushViewByTag* PushViewByTag(const UObject* WorldContextObject, APlayerController* PlayerController,
		UPARAM(meta = (Categories = "MVVMLayout.WidgetStack"))FGameplayTag WidgetStackTag,
		UPARAM(meta = (Categories = "MVVM.View"))FGameplayTag ViewTag,
		bool bIsNeedAddViewTypeToCache,
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
	FGameplayTag CachedViewTag = FGameplayTag{};
	UPROPERTY()
	bool bCachedIsNeedAddViewTypeToCache = false;
	UPROPERTY()
	bool bCachedFocusOnNewlyPushedWidget = true;
};
