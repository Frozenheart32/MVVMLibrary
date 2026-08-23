/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "GameplayCore/CommonMVVMHUD.h"

#include "PrimaryLayoutWidget.h"
#include "WindowSubsystem.h"
#include "Abstract/UIView.h"
#include "Global/MVVMGlobalTags.h"

ACommonMVVMHUD::ACommonMVVMHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACommonMVVMHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateAndRegisterPrimaryLayoutWidget();
	
	if(StartupViewTag.IsValid())
	{
		if(const auto WindowSubsystem = UWindowSubsystem::Get(this))
		{
			WindowSubsystem->PushViewToStackByTagAsync(GetOwningPlayerController(), MVVMLayoutTags::MVVMLayout_WidgetStack_GameHud, StartupViewTag,
				false, true, [this](EAsyncPushWidgetState State, UUIView* CreatedWidget)
				{
					switch (State)
					{
						case EAsyncPushWidgetState::OnCreatedBeforePush:
							break;
						case EAsyncPushWidgetState::AfterPush:
							StartPreCachingViewAndPopupTypes();
							break;
						default:
							break;
					}
				}
			);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ACommonHUD::BeginPlay. You have not selected a startup window tag in %s class"), *GetNameSafe(this));
		StartPreCachingViewAndPopupTypes();
	}
}

void ACommonMVVMHUD::CreateAndRegisterPrimaryLayoutWidget()
{
	checkf(PrimaryLayoutWidgetClass, TEXT("ACommonMVVMHUD::CreateAndRegisterPrimaryLayoutWidget. PrimaryLayoutWidgetClass is not seletected. Check this, pls"));
	if(const auto WindowSubsystem = UWindowSubsystem::Get(this))
	{
		WindowSubsystem->CreateAndRegisterCreatedPrimaryLayoutWidget(GetOwningPlayerController(), PrimaryLayoutWidgetClass);
	}
}

void ACommonMVVMHUD::StartPreCachingViewAndPopupTypes()
{
	if(PreCachingViewTags.IsEmpty() && PreCachingPopupTags.IsEmpty()) return;

	if(const auto WindowSubsystem = UWindowSubsystem::Get(this))
	{
		WindowSubsystem->StartPreCachingViewTypes(PreCachingViewTags);
		WindowSubsystem->StartPreCachingPopupTypes(PreCachingPopupTags);
	}
}
