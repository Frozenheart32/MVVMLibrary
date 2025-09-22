/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "Abstract/UIContextualModel.h"
#include "WorldModelRepositorySubsystem.h"
#include "ModelRepositorySubsystem.h"

UWorldModelRepositorySubsystem* UUIContextualModel::GetWorldModelRepository() const
{
	return WorldModelRepository.IsValid() ? WorldModelRepository.Get() : nullptr;
}

UModelRepositorySubsystem* UUIContextualModel::GetModelRepository() const
{
	return ModelRepository.IsValid() ? ModelRepository.Get() : nullptr;
}

void UUIContextualModel::SetWorldModelRepository(UWorldModelRepositorySubsystem* InWorldModelRepository)
{
	WorldModelRepository = InWorldModelRepository;
}

void UUIContextualModel::SetModelRepository(UModelRepositorySubsystem* InModelRepository)
{
	ModelRepository = InModelRepository;
}

void UUIContextualModel::OnInitModel()
{
	
}

void UUIContextualModel::OnDestroyModel()
{
	
}

void UUIContextualModel::K2_SetWorldModelRepository_Implementation(UWorldModelRepositorySubsystem* InWorldModelRepository)
{
	SetWorldModelRepository(InWorldModelRepository);
}

void UUIContextualModel::K2_SetModelRepository_Implementation(UModelRepositorySubsystem* InModelRepository)
{
	SetModelRepository(InModelRepository);
}

void UUIContextualModel::K2_OnInitModel_Implementation()
{
	OnInitModel();
}

void UUIContextualModel::K2_OnDestroyModel_Implementation()
{
	OnDestroyModel();
}
