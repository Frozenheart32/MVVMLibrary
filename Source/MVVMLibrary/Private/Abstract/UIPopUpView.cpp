/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "Abstract/UIPopupView.h"
#include "ModelRepositorySubsystem.h"
#include "WorldModelRepositorySubsystem.h"


void UUIPopupView::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	StartDestroyLogic();
}

void UUIPopupView::NativeOnDeactivated()
{
	if(SelfDestroyTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SelfDestroyTimerHandle);
	}
	
	bIsStartedPopup = false;

	Super::NativeOnDeactivated();
}

void UUIPopupView::InitializePopup(UModelRepositorySubsystem* InModelRepository, UWorldModelRepositorySubsystem* InWorldModelRepository)
{
	if(!ModelRepository.IsValid())
		ModelRepository = InModelRepository;

	if(!WorldModelRepository.IsValid())
		WorldModelRepository = InWorldModelRepository;
}

void UUIPopupView::FeedPopupData_Implementation(UObject* FeedDataObject)
{
	
}

void UUIPopupView::StartDestroyLogic()
{
	if(bIsStartedPopup) return;

	bIsStartedPopup = true;

	if(bUseSelfDestroyTimer)
	{
		GetWorld()->GetTimerManager().SetTimer(SelfDestroyTimerHandle,
			this,
			&ThisClass::OnDestroyTimerComplete,
			LifeSpan,
			false);
	}
}

bool UUIPopupView::IsStartedPopUp() const
{
	return bIsStartedPopup;
}

UModelRepositorySubsystem* UUIPopupView::GetModelRepository() const
{
	return ModelRepository.IsValid() ? ModelRepository.Get() : nullptr;
}

UWorldModelRepositorySubsystem* UUIPopupView::GetWorldModelRepository() const
{
	return WorldModelRepository.IsValid() ? WorldModelRepository.Get() : nullptr;
}

void UUIPopupView::OnDestroyTimerComplete()
{
	DeactivateWidget();
}
