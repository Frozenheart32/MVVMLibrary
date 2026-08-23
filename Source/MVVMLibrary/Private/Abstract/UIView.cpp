/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "Abstract/UIView.h"

#include "Abstract/UIViewModel.h"
#include "ModelRepositorySubsystem.h"
#include "WorldModelRepositorySubsystem.h"


void UUIView::NativeOnActivated()
{
	Super::NativeOnActivated();

	OnActivatedView.Broadcast();
}

void UUIView::NativeOnDeactivated()
{
	OnDeactivatedView.Broadcast();
	
	Super::NativeOnDeactivated();
}

void UUIView::NativeDestruct()
{
	OnDestroyView.Broadcast();
	
	Super::NativeDestruct();
}

void UUIView::InitializeView(UModelRepositorySubsystem* InModelRepository, UWorldModelRepositorySubsystem* InWorldModelRepository)
{
	if(bIsInitializedView) return;

	bIsInitializedView = true;
	checkf(IsValid(ViewModelClassType), TEXT("You have not selected a viewmodel class in view settings. View class name: %s"), *GetNameSafe(this));

	ViewModel = NewObject<UUIViewModel>(this, ViewModelClassType);
	ViewModel->SetModelRepository(InModelRepository);
	ViewModel->SetWorldModelRepository(InWorldModelRepository);
	ViewModel->InitializeViewModel(this);
}

bool UUIView::IsInitializedView() const
{
	return bIsInitializedView;
}

void UUIView::ShowView_Implementation()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUIView::HideView_Implementation()
{
	SetVisibility(ESlateVisibility::Collapsed);
}