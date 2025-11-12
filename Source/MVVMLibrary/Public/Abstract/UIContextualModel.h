/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "ObjectWithWorldContext.h"
#include "UIContextualModel.generated.h"

class UModelRepositorySubsystem;
class UWorldModelRepositorySubsystem;

/**
 * In the mvvm paradigm, a contextual model is used to store data that can be used during a single scene.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class MVVMLIBRARY_API UUIContextualModel : public UObjectWithWorldContext
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TWeakObjectPtr<UWorldModelRepositorySubsystem> WorldModelRepository = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UModelRepositorySubsystem> ModelRepository = nullptr;

protected:

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(BlueprintProtected), Category = "MVVM|ContextualModel")
	UWorldModelRepositorySubsystem* GetWorldModelRepository() const;
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(BlueprintProtected), Category = "MVVM|ContextualModel")
	UModelRepositorySubsystem* GetModelRepository() const;

	/**
	 * Should be overridden in C++ heirs
	 * @param InWorldModelRepository 
	 */
	UFUNCTION()
	virtual void SetWorldModelRepository(UWorldModelRepositorySubsystem* InWorldModelRepository);
	/**
	 * Should be overridden in C++ heirs
	 * @param InModelRepository 
	 */
	UFUNCTION()
	virtual void SetModelRepository(UModelRepositorySubsystem* InModelRepository);

	/**
	 * Should be overridden in C++ heirs
	 */
	UFUNCTION()
	virtual void OnInitModel();
	/**
	 * Should be overridden in C++ heirs
	 */
	UFUNCTION()
	virtual void OnDestroyModel();

protected:
	/**
	 * Sets the world model repository. You can use an override to get the required dependencies.
	 * Do not call this event yourself. For C++ heirs there is a virtual method without K2 prefix
	 * @param InWorldModelRepository 
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM|ContextualModel", meta=(ForceAsFunction, DisplayName = "SetWorldModelRepository", ScriptName = "SetWorldModelRepository"))
	void K2_SetWorldModelRepository(UWorldModelRepositorySubsystem* InWorldModelRepository);
	/**
	 * Sets the model repository. You can use an override to get the required dependencies.
	 * Do not call this event yourself. For C++ heirs there is a virtual method without K2 prefix
	 * @param InModelRepository 
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM|ContextualModel", meta=(ForceAsFunction, DisplayName = "SetModelRepository", ScriptName = "SetModelRepository"))
	void K2_SetModelRepository(UModelRepositorySubsystem* InModelRepository);

	/**
	 * Event called when the model is created.
	 * Do not call this event yourself. For C++ heirs there is a virtual method without K2 prefix
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM|ContextualModel", meta=(DisplayName = "OnInitModel", ScriptName = "OnInitModel"))
	void K2_OnInitModel();
	/**
	 * Event called when changing scene / shutdown game.
	 * Do not call this event yourself. For C++ heirs there is a virtual method without K2 prefix
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM|ContextualModel", meta=(DisplayName = "OnDestroyModel", ScriptName = "OnDestroyModel"))
	void K2_OnDestroyModel();

	friend class UWorldModelRepositorySubsystem;
};
