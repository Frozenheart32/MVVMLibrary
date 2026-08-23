/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "AsyncActions/AsyncAction_PushViewByTag.h"

#include "WindowSubsystem.h"

UAsyncAction_PushViewByTag* UAsyncAction_PushViewByTag::PushViewByTag(const UObject* WorldContextObject,
                                                                      APlayerController* PlayerController, FGameplayTag WidgetStackTag, FGameplayTag ViewTag,
                                                                      bool bIsNeedAddViewTypeToCache, bool bFocusOnNewlyPushedWidget)
{
	if(!WorldContextObject) return nullptr;
	
	checkf(ViewTag.IsValid() && WidgetStackTag.IsValid(), TEXT("UAsyncAction_PushViewByTag::PushViewByTag. Check node on %s class"), *GetNameSafe(WorldContextObject));

	if(GEngine)
	{
		if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushViewByTag* Node = NewObject<UAsyncAction_PushViewByTag>();
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = PlayerController;
			Node->CachedWidgetStackTag = WidgetStackTag;
			Node->CachedViewTag = ViewTag;
			Node->bCachedIsNeedAddViewTypeToCache = bIsNeedAddViewTypeToCache;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;
			
			Node->RegisterWithGameInstance(World);
			
			return Node;
		}
	}

	return nullptr;
}

void UAsyncAction_PushViewByTag::Activate()
{
	if(const auto WindowSubsystem = UWindowSubsystem::Get(CachedOwningWorld.Get()))
	{
		WindowSubsystem->PushViewToStackByTagAsync(CachedOwningPC.Get(), CachedWidgetStackTag, CachedViewTag, bCachedIsNeedAddViewTypeToCache, bCachedFocusOnNewlyPushedWidget,
			[this](EAsyncPushWidgetState InPushState, UUIView* PushedWidget)
		{
			switch (InPushState)
			{
				case EAsyncPushWidgetState::OnCreatedBeforePush:
					CreatedBeforePush.Broadcast(PushedWidget);
					break;
				case EAsyncPushWidgetState::AfterPush:
					AfterPush.Broadcast(PushedWidget);
					SetReadyToDestroy();
					break;
				default:
					break;
			}
		});
	}
	else
	{
		SetReadyToDestroy();
	}
}
