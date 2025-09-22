/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldModelRepositorySubsystem.generated.h"

class UModelRepositorySubsystem;
class UUIContextualModel;


/**
 * This class is used to store contextual models, i.e. models that live during one scene.
 */
UCLASS(NotBlueprintable, BlueprintType)
class MVVMLIBRARY_API UWorldModelRepositorySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	
	UPROPERTY()
	TMap<UClass*, UUIContextualModel*> ContextualModels;

	UPROPERTY()
	mutable TWeakObjectPtr<UModelRepositorySubsystem> ModelRepositorySubsystemCache = nullptr;

protected:
	/**
	 * Blueprint variant of GetContextualModel method
	 * @param OutContextualModel 
	 * @param ModelType Selected type
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, DisplayName = "GetContextualModel", Category = "MVVM|WorldModelRepository", meta=(DeterminesOutputType = "ModelType", DynamicOutputParam = "OutContextualModel"))
	void K2_GetContextualModel(UUIContextualModel*& OutContextualModel, UPARAM(meta=(AllowAbstract=false))TSubclassOf<UUIContextualModel> ModelType);

public:
	
	virtual void Deinitialize() override;

	/**
	 * C++ variant of GetContextualModel method.
	 * @param ModelType 
	 * @return 
	 */
	UFUNCTION()
	UUIContextualModel* GetContextualModel(TSubclassOf<UUIContextualModel> ModelType);

	/**
	 * C++ template variant of GetContextualModel method. Auto Cast to T type
	 * @tparam T Inheritor of UUIContextualModel class
	 * @return 
	 */
	template<class T = UUIContextualModel>
	T* GetContextualModel()
	{
		static_assert(TIsDerivedFrom<T, UUIContextualModel>::IsDerived, "GetContextualModel can only be used to create UUIContextualModel instances");

		return Cast<T>(GetContextualModel(T::StaticClass()));
	}

private:

	UFUNCTION()
	UModelRepositorySubsystem* GetModeRepositorySubsystem() const;
	
	UFUNCTION()
	UUIContextualModel* CreateContextualModel(const TSubclassOf<UUIContextualModel>& ModelType);
};
