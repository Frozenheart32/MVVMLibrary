/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "Abstract/UISessionModel.h"
#include "ModelRepositorySubsystem.h"

UModelRepositorySubsystem* UUISessionModel::GetModelRepository() const
{
	return ModelRepository.IsValid() ? ModelRepository.Get() : nullptr;
}

void UUISessionModel::SetModelRepository(UModelRepositorySubsystem* InModelRepository)
{
	ModelRepository = InModelRepository;
	K2_SetModelRepository(InModelRepository);
}

void UUISessionModel::StartSession()
{
	K2_StartSession();
}

void UUISessionModel::EndSession()
{
	K2_EndSession();
}
