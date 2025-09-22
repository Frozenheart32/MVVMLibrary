/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "WorldModelRepositorySubsystem.h"

#include "Abstract/UIContextualModel.h"
#include "ModelRepositorySubsystem.h"

void UWorldModelRepositorySubsystem::K2_GetContextualModel(UUIContextualModel*& OutContextualModel,
	TSubclassOf<UUIContextualModel> ModelType)
{
	OutContextualModel = GetContextualModel(MoveTemp(ModelType));
}

void UWorldModelRepositorySubsystem::Deinitialize()
{
	for (const auto& [ModelType, ContextualModel] : ContextualModels)
	{
		if(ContextualModel)
			ContextualModel->K2_OnDestroyModel();
	}

	ContextualModels.Empty();
	
	Super::Deinitialize();
}

UUIContextualModel* UWorldModelRepositorySubsystem::GetContextualModel(TSubclassOf<UUIContextualModel> ModelType)
{
	if(!IsValid(ModelType)) return nullptr;
	
	if(ContextualModels.Contains(ModelType))
	{
		return ContextualModels[ModelType];
	}

	return CreateContextualModel(ModelType);
}

UUIContextualModel* UWorldModelRepositorySubsystem::CreateContextualModel(const TSubclassOf<UUIContextualModel>& ModelType)
{
	UUIContextualModel* NewModel = NewObject<UUIContextualModel>(this, ModelType);

	const auto ModelRepository = GetModeRepositorySubsystem();
	check(ModelRepository);
	ContextualModels.Add(ModelType, NewModel);
	NewModel->K2_SetModelRepository(ModelRepository);
	NewModel->K2_SetWorldModelRepository(this);

	NewModel->K2_OnInitModel();

	return NewModel;
}

UModelRepositorySubsystem* UWorldModelRepositorySubsystem::GetModeRepositorySubsystem() const
{
	if(ModelRepositorySubsystemCache.IsValid())
	{
		return ModelRepositorySubsystemCache.Get();
	}

	if(GetWorld())
	{
		ModelRepositorySubsystemCache = GetWorld()->GetGameInstance()->GetSubsystem<UModelRepositorySubsystem>();
		return ModelRepositorySubsystemCache.Get();
	}

	return nullptr;
}
