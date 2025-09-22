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
}

void UUISessionModel::StartSession()
{
	
}

void UUISessionModel::EndSession()
{
	
}

void UUISessionModel::K2_SetModelRepository_Implementation(UModelRepositorySubsystem* InModelRepository)
{
	SetModelRepository(InModelRepository);
}

void UUISessionModel::K2_StartSession_Implementation()
{
	StartSession();
}

void UUISessionModel::K2_EndSession_Implementation()
{
	EndSession();
}
