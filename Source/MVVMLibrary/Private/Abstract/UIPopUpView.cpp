/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "Abstract/UIPopUpView.h"
#include "ModelRepositorySubsystem.h"
#include "WorldModelRepositorySubsystem.h"

void UUIPopUpView::NativeDestruct()
{
	OnDestroyPopUp.Broadcast();
	
	Super::NativeDestruct();
}

void UUIPopUpView::InitializePopUp(UModelRepositorySubsystem* InModelRepository,
	UWorldModelRepositorySubsystem* InWorldModelRepository)
{
	if(bIsInitializedPopUp) return;

	bIsInitializedPopUp = true;

	ModelRepository = InModelRepository;
	WorldModelRepository = InWorldModelRepository;

	K2_InitializePopUp(InModelRepository, InWorldModelRepository);
	
	if(bUseSelfDestroyTimer)
	{
		GetWorld()->GetTimerManager().SetTimer(SelfDestroyTimerHandle,
			this,
			&ThisClass::OnDestroyTimerComplete,
			LifeSpan,
			false);
	}
}

EUILayer UUIPopUpView::GetUILayer() const
{
	return ViewLayer;
}

bool UUIPopUpView::IsInitializedPopUp() const
{
	return bIsInitializedPopUp;
}

UModelRepositorySubsystem* UUIPopUpView::GetModelRepository() const
{
	return ModelRepository.IsValid() ? ModelRepository.Get() : nullptr;
}

UWorldModelRepositorySubsystem* UUIPopUpView::GetWorldModelRepository() const
{
	return WorldModelRepository.IsValid() ? WorldModelRepository.Get() : nullptr;
}

void UUIPopUpView::OnDestroyTimerComplete()
{
	RemoveFromParent();
}
