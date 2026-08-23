/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "AsyncActions/AsyncAction_PushPopupByTag.h"

#include "WindowSubsystem.h"
#include "Abstract/UIPopupView.h"

UAsyncAction_PushPopupByTag* UAsyncAction_PushPopupByTag::PushPopupByTag(const UObject* WorldContextObject,
                                                                         APlayerController* PlayerController, FGameplayTag WidgetStackTag, FGameplayTag PopupTag, UObject* FeedDataObject,
                                                                         bool bIsNeedAddViewTypeToCache, bool bFocusOnNewlyPushedWidget)
{
	if(!WorldContextObject) return nullptr;
	
	checkf(PopupTag.IsValid() && WidgetStackTag.IsValid(), TEXT("UAsyncAction_PushPopupByTag::PushPopupByTag. Check node on %s class"), *GetNameSafe(WorldContextObject));

	if(GEngine)
	{
		if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushPopupByTag* Node = NewObject<UAsyncAction_PushPopupByTag>();
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = PlayerController;
			Node->CachedWidgetStackTag = WidgetStackTag;
			Node->CachedPopupTag = PopupTag;
			Node->CachedFeedDataObject = FeedDataObject;
			Node->bCachedIsNeedAddViewTypeToCache = bIsNeedAddViewTypeToCache;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;
			
			Node->RegisterWithGameInstance(World);
			
			return Node;
		}
	}

	return nullptr;
}

void UAsyncAction_PushPopupByTag::Activate()
{
	if(const auto WindowSubsystem = UWindowSubsystem::Get(CachedOwningWorld.Get()))
	{
		WindowSubsystem->PushPopupToStackByTagAsync(CachedOwningPC.Get(), CachedWidgetStackTag, CachedPopupTag, bCachedIsNeedAddViewTypeToCache, bCachedFocusOnNewlyPushedWidget,CachedFeedDataObject,
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

void UAsyncAction_PushPopupByTag::FinishAction()
{
	CachedFeedDataObject = nullptr;
	SetReadyToDestroy();
}
