/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "ModelRepositorySubsystem.h"

#include "Abstract/UISessionModel.h"

UModelRepositorySubsystem* UModelRepositorySubsystem::Get(const UObject* WorldContextObject)
{
	if(GEngine)
	{
		const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UModelRepositorySubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

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
			SessionModel->EndSession();
	}

	SessionModels.Empty();
}

UUISessionModel* UModelRepositorySubsystem::CreateSessionModel(const TSubclassOf<UUISessionModel>& ModelType)
{
	UUISessionModel* NewModel = NewObject<UUISessionModel>(this, ModelType);
	SessionModels.Add(ModelType, NewModel);
	NewModel->SetModelRepository(this);
	NewModel->StartSession();

	return NewModel;
}
