/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "Abstract/UIView.h"

#include "Abstract/UIViewModel.h"
#include "ModelRepositorySubsystem.h"
#include "WorldModelRepositorySubsystem.h"


void UUIView::NativeDestruct()
{
	OnDestroyView.Broadcast();
	
	Super::NativeDestruct();
}

void UUIView::InitializeView(UModelRepositorySubsystem* InModelRepository,
                             UWorldModelRepositorySubsystem* InWorldModelRepository)
{
	if(bIsInitializedView) return;

	bIsInitializedView = true;
	checkf(IsValid(ViewModelClassType), TEXT("You have not selected a viewmodel class in view settings. View class name: %s"), *GetNameSafe(this));

	ViewModel = NewObject<UUIViewModel>(this, ViewModelClassType);
	ViewModel->K2_SetModelRepository(InModelRepository);
	ViewModel->K2_SetWorldModelRepository(InWorldModelRepository);
	ViewModel->K2_InitializeViewModel(this);
}

EUILayer UUIView::GetUILayer() const
{
	return ViewLayer;
}

bool UUIView::IsInitializedView() const
{
	return bIsInitializedView;
}

void UUIView::K2_ShowView_Implementation()
{
	ShowView();
}

void UUIView::K2_HideView_Implementation()
{
	HideView();
}

void UUIView::ShowView()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUIView::HideView()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
