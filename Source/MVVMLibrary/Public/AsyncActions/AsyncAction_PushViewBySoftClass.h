/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushViewBySoftClass.generated.h"

class UUIView;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushViewBySoftClassDelegate, UUIView*, PushedView);

/**
* Used for push views. Uses the view's softclass directly. Not cacheable for view types.
*/
UCLASS()
class MVVMLIBRARY_API UAsyncAction_PushViewBySoftClass : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:

	UPROPERTY(BlueprintAssignable)
	FOnPushViewBySoftClassDelegate CreatedBeforePush;
	UPROPERTY(BlueprintAssignable)
	FOnPushViewBySoftClassDelegate AfterPush;

	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push View By SoftClass Async Action"))
	static UAsyncAction_PushViewBySoftClass* PushViewBySoftClass(const UObject* WorldContextObject, APlayerController* PlayerController,
		UPARAM(meta = (Categories = "MVVMLayout.WidgetStack"))FGameplayTag WidgetStackTag,
		TSoftClassPtr<UUIView> ViewSoftClass,
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
	TSoftClassPtr<UUIView> CachedViewSoftType;
	UPROPERTY()
	bool bCachedFocusOnNewlyPushedWidget = true;
};
