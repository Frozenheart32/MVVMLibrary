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
	K2_SetWorldModelRepository(InWorldModelRepository);
}

void UUIContextualModel::SetModelRepository(UModelRepositorySubsystem* InModelRepository)
{
	ModelRepository = InModelRepository;
	K2_SetModelRepository(InModelRepository);
}

void UUIContextualModel::OnInitModel()
{
	K2_OnInitModel();
}

void UUIContextualModel::OnDestroyModel()
{
	K2_OnDestroyModel();
}