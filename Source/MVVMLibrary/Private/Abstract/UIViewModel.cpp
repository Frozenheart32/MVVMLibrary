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
	
}

void UUIViewModel::SetModelRepository(UModelRepositorySubsystem* InModelRepository)
{
	ModelRepository = InModelRepository;
}

void UUIViewModel::SetWorldModelRepository(UWorldModelRepositorySubsystem* InWorldModelRepository)
{
	WorldModelRepository = InWorldModelRepository;
}

void UUIViewModel::InitializeViewModel(UUIView* View)
{
	OwnerView = View;
}

void UUIViewModel::K2_OnDestroyViewModel_Implementation()
{
	//Unsubscribe from OnDestroyView event
	if(const auto View = GetOwnerView())
	{
		View->OnDestroyView.RemoveDynamic(this, &UUIViewModel::K2_OnDestroyViewModel);
	}
	
	OnDestroyViewModel();
}

void UUIViewModel::K2_SetModelRepository_Implementation(UModelRepositorySubsystem* InModelRepository)
{
	SetModelRepository(InModelRepository);
}

void UUIViewModel::K2_SetWorldModelRepository_Implementation(UWorldModelRepositorySubsystem* InWorldModelRepository)
{
	SetWorldModelRepository(InWorldModelRepository);
}

void UUIViewModel::K2_InitializeViewModel_Implementation(UUIView* View)
{
	check(View);
	InitializeViewModel(View);

	//Subscribe on OnDestroyView event
	View->OnDestroyView.AddDynamic(this, &UUIViewModel::K2_OnDestroyViewModel);
}
