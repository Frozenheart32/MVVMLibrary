/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "AsyncActions/AsyncAction_PushViewBySoftClass.h"

#include "WindowSubsystem.h"

UAsyncAction_PushViewBySoftClass* UAsyncAction_PushViewBySoftClass::PushViewBySoftClass(const UObject* WorldContextObject,
                                                                                  APlayerController* PlayerController, FGameplayTag WidgetStackTag, TSoftClassPtr<UUIView> ViewSoftClass,
                                                                                  bool bFocusOnNewlyPushedWidget)
{
	if(!WorldContextObject) return nullptr;
	
	checkf(!ViewSoftClass.IsNull() && WidgetStackTag.IsValid(), TEXT("UAsyncAction_PushViewBySoftClass::PushViewBySoftClass. Check node on %s class"), *GetNameSafe(WorldContextObject));

	if(GEngine)
	{
		if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushViewBySoftClass* Node = NewObject<UAsyncAction_PushViewBySoftClass>();
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = PlayerController;
			Node->CachedWidgetStackTag = WidgetStackTag;
			Node->CachedViewSoftType = ViewSoftClass;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;
			
			Node->RegisterWithGameInstance(World);
			
			return Node;
		}
	}

	return nullptr;
}

void UAsyncAction_PushViewBySoftClass::Activate()
{
	if(const auto WindowSubsystem = UWindowSubsystem::Get(CachedOwningWorld.Get()))
	{
		WindowSubsystem->PushViewToStackBySoftClassAsync(CachedOwningPC.Get(), CachedWidgetStackTag, CachedViewSoftType, bCachedFocusOnNewlyPushedWidget,
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
