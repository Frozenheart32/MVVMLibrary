/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "AsyncActions/AsyncAction_PushPopupBySoftClass.h"

#include "WindowSubsystem.h"
#include "Abstract/UIPopupView.h"

UAsyncAction_PushPopupBySoftClass* UAsyncAction_PushPopupBySoftClass::PushViewByTag(const UObject* WorldContextObject,
                                                                                    APlayerController* PlayerController,
                                                                                    FGameplayTag WidgetStackTag, TSoftClassPtr<UUIPopupView> PopupSoftClass,
                                                                                    UObject* FeedDataObject, bool bFocusOnNewlyPushedWidget)
{
	if(!WorldContextObject) return nullptr;
	
	checkf(!PopupSoftClass.IsNull() && WidgetStackTag.IsValid(), TEXT("UAsyncAction_PushPopupBySoftClass::PushViewByTag. Check node on %s class"), *GetNameSafe(WorldContextObject));

	if(GEngine)
	{
		if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushPopupBySoftClass* Node = NewObject<UAsyncAction_PushPopupBySoftClass>();
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = PlayerController;
			Node->CachedWidgetStackTag = WidgetStackTag;
			Node->CachedFeedDataObject = FeedDataObject;
			Node->CachedPopupSoftType = PopupSoftClass;
			
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;
			
			Node->RegisterWithGameInstance(World);
			
			return Node;
		}
	}

	return nullptr;
}

void UAsyncAction_PushPopupBySoftClass::Activate()
{
	if(const auto WindowSubsystem = UWindowSubsystem::Get(CachedOwningWorld.Get()))
	{
		WindowSubsystem->PushPopupToStackBySoftClassAsync(CachedOwningPC.Get(), CachedWidgetStackTag, CachedPopupSoftType, bCachedFocusOnNewlyPushedWidget, CachedFeedDataObject,
			[this](EAsyncPushWidgetState InPushState, UUIPopupView* PushedWidget)
		{
			switch (InPushState)
			{
				case EAsyncPushWidgetState::OnCreatedBeforePush:
					CreatedBeforePush.Broadcast(PushedWidget);
					break;
				case EAsyncPushWidgetState::AfterPush:
					AfterPush.Broadcast(PushedWidget);
					FinishAction();
					break;
				default:
					break;
			}
		});
	}
	else
	{
		FinishAction();
	}
}

void UAsyncAction_PushPopupBySoftClass::FinishAction()
{
	CachedFeedDataObject = nullptr;
	SetReadyToDestroy();
}
