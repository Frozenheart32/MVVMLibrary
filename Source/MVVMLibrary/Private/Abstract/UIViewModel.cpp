/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "Abstract/UIViewModel.h"
#include "Abstract/UIView.h"
#include "Kismet/GameplayStatics.h"
#include "WindowSubsystem.h"
#include "WorldModelRepositorySubsystem.h"
#include "ModelRepositorySubsystem.h"

UModelRepositorySubsystem* UUIViewModel::GetModelRepository() const
{
	return ModelRepository.IsValid() ? ModelRepository.Get() : nullptr;
}

UWorldModelRepositorySubsystem* UUIViewModel::GetWorldModelRepository() const
{
	return WorldModelRepository.IsValid() ? WorldModelRepository.Get() : nullptr;
}

UWindowSubsystem* UUIViewModel::GetWindowSubsystem() const
{
	if(const auto WorldContextObject = GetWorldContext())
	{
		return WorldContextObject->GetSubsystem<UWindowSubsystem>();
	}

	return nullptr;
}

APlayerController* UUIViewModel::GetOwningPlayer() const
{
	if(OwnerView.IsValid())
	{
		return OwnerView->GetOwningPlayer();
	}

	if(const auto WorldContextObject = GetWorldContext())
	{
		return UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	}

	return nullptr;
}

UUIView* UUIViewModel::GetOwnerView() const
{
	return OwnerView.IsValid() ? OwnerView.Get() : nullptr;
}

void UUIViewModel::OnDestroyViewModel()
{
	//Unsubscribe from OnDestroyView event
	if(const auto View = GetOwnerView())
	{
		View->OnDestroyView.RemoveDynamic(this, &UUIViewModel::OnDestroyViewModel);
	}
	
	K2_OnDestroyViewModel();
}

void UUIViewModel::SetModelRepository(UModelRepositorySubsystem* InModelRepository)
{
	ModelRepository = InModelRepository;

	K2_SetModelRepository(InModelRepository);
}

void UUIViewModel::SetWorldModelRepository(UWorldModelRepositorySubsystem* InWorldModelRepository)
{
	WorldModelRepository = InWorldModelRepository;

	K2_SetWorldModelRepository(InWorldModelRepository);
}

void UUIViewModel::InitializeViewModel(UUIView* View)
{
	check(View);
	OwnerView = View;

	//Subscribe on OnDestroyView event
	View->OnDestroyView.AddDynamic(this, &UUIViewModel::OnDestroyViewModel);
	
	K2_InitializeViewModel(View);
}
