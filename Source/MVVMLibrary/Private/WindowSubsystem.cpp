/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "WindowSubsystem.h"

#include "Abstract/UIView.h"
#include "WorldModelRepositorySubsystem.h"
#include "ModelRepositorySubsystem.h"
#include "PrimaryLayoutWidget.h"
#include "Abstract/UIPopupView.h"
#include "Components/PanelWidget.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Util/MVVMPluginFunctionLibrary.h"
#include "Widgets/CommonActivatableWidgetContainer.h"


void UWindowSubsystem::K2_PushViewToStackByTagAsync(APlayerController* OwningPlayerController,
	FGameplayTag InWidgetStackTag, FGameplayTag InViewTag, bool bIsNeedAddTypeToCache,
	bool bFocusOnNewlyPushedWidget)
{
	PushViewToStackByTagAsync(OwningPlayerController, InWidgetStackTag, InViewTag, bIsNeedAddTypeToCache, bFocusOnNewlyPushedWidget);
}

void UWindowSubsystem::K2_PushViewToStackBySoftClassAsync(APlayerController* OwningPlayerController,
	FGameplayTag InWidgetStackTag, TSoftClassPtr<UUIView> ViewSoftType, bool bFocusOnNewlyPushedWidget)
{
	PushViewToStackBySoftClassAsync(OwningPlayerController, InWidgetStackTag, ViewSoftType, bFocusOnNewlyPushedWidget);
}

void UWindowSubsystem::K2_PushPopupToStackByTagAsync(APlayerController* OwningPlayerController,
	FGameplayTag InWidgetStackTag, FGameplayTag InPopupTag, bool bIsNeedAddTypeToCache,
	bool bFocusOnNewlyPushedWidget, UObject* InFeedDataObject)
{
	PushPopupToStackByTagAsync(OwningPlayerController, InWidgetStackTag, InPopupTag, bIsNeedAddTypeToCache, bFocusOnNewlyPushedWidget, InFeedDataObject);
}

void UWindowSubsystem::K2_PushPopupToStackBySoftClassAsync(APlayerController* OwningPlayerController,
	FGameplayTag InWidgetStackTag, TSoftClassPtr<UUIPopupView> PopupSoftType,
	bool bFocusOnNewlyPushedWidget, UObject* InFeedDataObject)
{
	PushPopupToStackBySoftClassAsync(OwningPlayerController, InWidgetStackTag, PopupSoftType, bFocusOnNewlyPushedWidget, InFeedDataObject);
}

UWindowSubsystem* UWindowSubsystem::Get(const UObject* WorldContextObject)
{
	if(GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return World->GetSubsystem<UWindowSubsystem>();
	}

	return nullptr;
}

void UWindowSubsystem::CreateAndRegisterCreatedPrimaryLayoutWidget(APlayerController* OwningPlayerController, const TSubclassOf<UPrimaryLayoutWidget>& InLayoutWidgetClass)
{
	if(PrimaryLayoutWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWindowSubsystem::RegisterCreatedPrimaryLayoutWidget. Primary Layout Widget already exits"));
		return;
	}
	
	check(InLayoutWidgetClass);
	PrimaryLayoutWidget = CreateWidget<UPrimaryLayoutWidget>(OwningPlayerController, InLayoutWidgetClass);
	PrimaryLayoutWidget->AddToViewport();
}

void UWindowSubsystem::StartPreCachingViewTypes(const TArray<FGameplayTag>& InCachingViewTags)
{
	if(InCachingViewTags.IsEmpty()) return;

	for (const auto& ViewTag : InCachingViewTags)
	{
		if(CachedViewTypes.Contains(ViewTag)) continue;
		
		const auto SoftViewClass = UMVVMPluginFunctionLibrary::GetViewSoftClassByTag(ViewTag);
		
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		SoftViewClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, SoftViewClass, ViewTag]()
			{
				if(CachedViewTypes.Contains(ViewTag)) return;
				
				CachedViewTypes.Add(ViewTag, SoftViewClass.Get());
			}
		));
	} 
}

void UWindowSubsystem::StartPreCachingPopupTypes(const TArray<FGameplayTag>& InCachingPopupTags)
{
	if(InCachingPopupTags.IsEmpty()) return;

	for (const auto& PopupTag : InCachingPopupTags)
	{
		if(CachedPopupTypes.Contains(PopupTag)) continue;
		
		const auto SoftPopupClass = UMVVMPluginFunctionLibrary::GetPopupSoftClassByTag(PopupTag);
		
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		SoftPopupClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, SoftPopupClass, PopupTag]()
			{
				if(CachedPopupTypes.Contains(PopupTag)) return;
				
				CachedPopupTypes.Add(PopupTag, SoftPopupClass.Get());
			}
		));
	} 
}

void UWindowSubsystem::PushViewToStackByTagAsync(APlayerController* OwningPlayerController, const FGameplayTag& InWidgetStackTag,
                                                 const FGameplayTag& InViewTag, bool bIsNeedAddViewTypeToCache, bool bFocusOnNewlyPushedWidget,
                                                 TFunction<void(EAsyncPushWidgetState, UUIView*)> AsyncPushStateCallback)
{
	check(InViewTag.IsValid());
	check(InWidgetStackTag.IsValid());
	
	if(!PrimaryLayoutWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("UWindowSubsystem::PushViewToStackAsync. PrimaryLayoutWidget is not valid. Creating View is canceled"));
		return;
	}
	
	if(CachedViewTypes.Contains(InViewTag))
	{
		CreateView(OwningPlayerController, CachedViewTypes[InViewTag], InWidgetStackTag, bFocusOnNewlyPushedWidget, AsyncPushStateCallback);
		return;
	}

	const auto SoftViewClass = UMVVMPluginFunctionLibrary::GetViewSoftClassByTag(InViewTag);
	
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		SoftViewClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, OwningPlayerController, SoftViewClass, InWidgetStackTag, InViewTag, bIsNeedAddViewTypeToCache, bFocusOnNewlyPushedWidget, AsyncPushStateCallback]()
			{
				const TSubclassOf<UUIView> LoadedViewClass = SoftViewClass.Get();
				CreateView(OwningPlayerController, LoadedViewClass, InWidgetStackTag, bFocusOnNewlyPushedWidget, AsyncPushStateCallback);

				if(bIsNeedAddViewTypeToCache && !CachedViewTypes.Contains(InViewTag))
				{
					CachedViewTypes.Add(InViewTag, LoadedViewClass);
				}
			}
		));
}

void UWindowSubsystem::PushViewToStackBySoftClassAsync(APlayerController* OwningPlayerController, const FGameplayTag& InWidgetStackTag,
	const TSoftClassPtr<UUIView>& ViewSoftType, bool bFocusOnNewlyPushedWidget, TFunction<void(EAsyncPushWidgetState, UUIView*)> AsyncPushStateCallback)
{
	check(!ViewSoftType.IsNull());
	check(InWidgetStackTag.IsValid());
	
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		ViewSoftType.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[OwningPlayerController, ViewSoftType, this, InWidgetStackTag, bFocusOnNewlyPushedWidget, AsyncPushStateCallback]()
			{
				const TSubclassOf<UUIView> LoadedViewClass = ViewSoftType.Get();
				CreateView(OwningPlayerController, LoadedViewClass, InWidgetStackTag, bFocusOnNewlyPushedWidget, AsyncPushStateCallback);
			}
		));
}

void UWindowSubsystem::PushPopupToStackByTagAsync(APlayerController* OwningPlayerController, const FGameplayTag& InWidgetStackTag, const FGameplayTag& InPopupTag,
                                             bool bIsNeedAddTypeToCache, bool bFocusOnNewlyPushedWidget, UObject* InFeedDataObject,
                                             TFunction<void(EAsyncPushWidgetState, UUIPopupView*)> AsyncPushStateCallback)
{
	check(InPopupTag.IsValid());
	check(InWidgetStackTag.IsValid());
	
	if(!PrimaryLayoutWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("UWindowSubsystem::PushPopupToStackAsync. PrimaryLayoutWidget is not valid. Creating View is canceled"));
		return;
	}

	if(CachedPopupTypes.Contains(InPopupTag))
	{
		CreatePopup(OwningPlayerController, CachedPopupTypes[InPopupTag], InWidgetStackTag, bFocusOnNewlyPushedWidget, InFeedDataObject, AsyncPushStateCallback);
		return;
	}

	const auto SoftPopupClass = UMVVMPluginFunctionLibrary::GetPopupSoftClassByTag(InPopupTag);
	
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		SoftPopupClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, OwningPlayerController, SoftPopupClass, InWidgetStackTag, InPopupTag, bIsNeedAddTypeToCache, bFocusOnNewlyPushedWidget, InFeedDataObject, AsyncPushStateCallback]()
			{
				const TSubclassOf<UUIPopupView> LoadedPopupClass = SoftPopupClass.Get();
				CreatePopup(OwningPlayerController, LoadedPopupClass, InWidgetStackTag, bFocusOnNewlyPushedWidget, InFeedDataObject, AsyncPushStateCallback);

				if(bIsNeedAddTypeToCache && !CachedPopupTypes.Contains(InPopupTag))
				{
					CachedPopupTypes.Add(InPopupTag, LoadedPopupClass);
				}
			}
		));
}

void UWindowSubsystem::PushPopupToStackBySoftClassAsync(APlayerController* OwningPlayerController, const FGameplayTag& InWidgetStackTag,
	const TSoftClassPtr<UUIPopupView>& PopupSoftType, bool bFocusOnNewlyPushedWidget, UObject* InFeedDataObject,
	TFunction<void(EAsyncPushWidgetState, UUIPopupView*)> AsyncPushStateCallback)
{
	check(!PopupSoftType.IsNull());
	check(InWidgetStackTag.IsValid());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		PopupSoftType.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, OwningPlayerController, PopupSoftType, InWidgetStackTag, bFocusOnNewlyPushedWidget, InFeedDataObject, AsyncPushStateCallback]()
			{
				const TSubclassOf<UUIPopupView> LoadedPopupClass = PopupSoftType.Get();
				CreatePopup(OwningPlayerController, LoadedPopupClass, InWidgetStackTag, bFocusOnNewlyPushedWidget, InFeedDataObject, AsyncPushStateCallback);
			}
		));
}

void UWindowSubsystem::InitializeExistsView(UUIView* ExistedView)
{
	if(IsRunningDedicatedServer() || !ExistedView || ExistedView->IsInitializedView()) return;

	ExistedView->InitializeView(GetModelRepositorySubsystem(), GetWorldModelRepositorySubsystem());
}

void UWindowSubsystem::HideAllWindows()
{
	if(bIsHiddenAllWindows) return;
	
	bIsHiddenAllWindows = true;
	//TODO: Change logic later
	/*
	for (const auto& WindowPair : OpenedWindows)
	{
		WindowPair.Value->HideView();
	}
	*/
}

void UWindowSubsystem::ShowAllWindows()
{
	if(!bIsHiddenAllWindows) return;
	
	bIsHiddenAllWindows = false;
	//TODO: Change logic later
	/*
	for (const auto& WindowPair : OpenedWindows)
	{
		WindowPair.Value->ShowView();
	}
	*/
}

void UWindowSubsystem::CreateView(APlayerController* OwningPlayerController, const TSubclassOf<UUIView>& LoadedViewClass, const FGameplayTag& InWidgetStackTag, bool bFocusOnNewlyPushedWidget, TFunction<void(EAsyncPushWidgetState, UUIView*)> AsyncPushStateCallback)
{
	check(LoadedViewClass);

	if(!OwningPlayerController)
	{
		OwningPlayerController = UGameplayStatics::GetPlayerController(this, 0);;
	}

	const auto FoundWidgetStack = PrimaryLayoutWidget->FindWidgetStackByTag(InWidgetStackTag);
	const auto CreatedWidget = FoundWidgetStack->AddWidget<UUIView>(
		LoadedViewClass,
		[this, OwningPlayerController, AsyncPushStateCallback](UUIView& CreatedWidgetInstance)
		{
			CreatedWidgetInstance.SetOwningPlayer(OwningPlayerController);
			CreatedWidgetInstance.InitializeView(GetModelRepositorySubsystem(), GetWorldModelRepositorySubsystem());

			if(AsyncPushStateCallback)
				AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
		}
	);

	if(bFocusOnNewlyPushedWidget)
	{
		if(UWidget* WidgetToFocus = CreatedWidget->GetDesiredFocusTarget())
		{
			WidgetToFocus->SetFocus();
		}
	}

	if(AsyncPushStateCallback)
		AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
}

void UWindowSubsystem::CreatePopup(APlayerController* OwningPlayerController, const TSubclassOf<UUIPopupView>& LoadedPopupClass, const FGameplayTag& InWidgetStackTag, bool bFocusOnNewlyPushedWidget, UObject* InFeedDataObject,
	TFunction<void(EAsyncPushWidgetState, UUIPopupView*)> AsyncPushStateCallback)
{
	check(LoadedPopupClass);

	if(!OwningPlayerController)
	{
		OwningPlayerController = UGameplayStatics::GetPlayerController(this, 0);;
	}

	const auto FoundWidgetStack = PrimaryLayoutWidget->FindWidgetStackByTag(InWidgetStackTag);
	const auto CreatedWidget = FoundWidgetStack->AddWidget<UUIPopupView>(
		LoadedPopupClass,
		[this, OwningPlayerController, AsyncPushStateCallback, InFeedDataObject](UUIPopupView& CreatedWidgetInstance)
		{
			CreatedWidgetInstance.SetOwningPlayer(OwningPlayerController);
			CreatedWidgetInstance.InitializePopup(GetModelRepositorySubsystem(), GetWorldModelRepositorySubsystem());
			CreatedWidgetInstance.FeedPopupData(InFeedDataObject);

			if(AsyncPushStateCallback)
				AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
		}
	);

	if(bFocusOnNewlyPushedWidget)
	{
		if(UWidget* WidgetToFocus = CreatedWidget->GetDesiredFocusTarget())
		{
			WidgetToFocus->SetFocus();
		}
	}

	if(AsyncPushStateCallback)
		AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
}

UModelRepositorySubsystem* UWindowSubsystem::GetModelRepositorySubsystem() const
{
	if(!CachedModelRepository.IsValid())
	{
		if(GetWorld())
		{
			CachedModelRepository = GetWorld()->GetGameInstance()->GetSubsystem<UModelRepositorySubsystem>();
		}
	}
	
	return CachedModelRepository.Get();
}

UWorldModelRepositorySubsystem* UWindowSubsystem::GetWorldModelRepositorySubsystem() const
{
	if(!CachedWorldModelRepository.IsValid())
	{
		if(GetWorld())
		{
			CachedWorldModelRepository = GetWorld()->GetSubsystem<UWorldModelRepositorySubsystem>();
		}
	}
	
	return CachedWorldModelRepository.Get();
}
