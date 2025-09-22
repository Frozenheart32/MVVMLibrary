/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "ModelRepositorySubsystem.h"

#include "Abstract/UISessionModel.h"

void UModelRepositorySubsystem::K2_GetSessionModel(UUISessionModel*& OutSessionModel,
	TSubclassOf<UUISessionModel> ModelType)
{
	OutSessionModel = GetSessionModel(MoveTemp(ModelType));
}

UUISessionModel* UModelRepositorySubsystem::GetSessionModel(TSubclassOf<UUISessionModel> ModelType)
{
	if(!IsValid(ModelType)) return nullptr;
	
	if(SessionModels.Contains(ModelType))
	{
		return SessionModels[ModelType];
	}

	return CreateSessionModel(ModelType);
}

void UModelRepositorySubsystem::CloseSession()
{
	for (const auto& [ModelType, SessionModel] : SessionModels)
	{
		if(SessionModel)
			SessionModel->K2_EndSession();
	}

	SessionModels.Empty();
}

UUISessionModel* UModelRepositorySubsystem::CreateSessionModel(const TSubclassOf<UUISessionModel>& ModelType)
{
	UUISessionModel* NewModel = NewObject<UUISessionModel>(this, ModelType);
	SessionModels.Add(ModelType, NewModel);
	NewModel->K2_SetModelRepository(this);
	NewModel->K2_StartSession();

	return NewModel;
}
