/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "ObjectWithWorldContext.h"
#include "UISessionModel.generated.h"

class UModelRepositorySubsystem;

/**
 * In the MVVM paradigm, the session model is used to store data that can be used throughout the entire runtime.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class MVVMLIBRARY_API UUISessionModel : public UObjectWithWorldContext
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TWeakObjectPtr<UModelRepositorySubsystem> ModelRepository = nullptr;

protected:

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(BlueprintProtected), Category = "MVVM|SessionModel")
	UModelRepositorySubsystem* GetModelRepository() const;

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
	virtual void StartSession();
	/**
	 * Should be overridden in C++ heirs
	 */
	UFUNCTION()
	virtual void EndSession();

protected:
	
	/**
	 * Event sets the model repository. You can use an override to get the required dependencies.
	 * Do not call this event yourself. For C++ there is a virtual method without K2 prefix
	 * @param InModelRepository 
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM|SessionModel", meta=(ForceAsFunction, DisplayName = "SetModelRepository", ScriptName = "SetModelRepository"))
	void K2_SetModelRepository(UModelRepositorySubsystem* InModelRepository);

	/**
	 * Event called when the model is created.
	 * Do not call this event yourself. For C++ there is a virtual method without K2 prefix
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM|SessionModel", meta=(DisplayName = "StartSession", ScriptName = "StartSession"))
	void K2_StartSession();
	
	/**
	 * Event called when started shutdown game.
	 * Do not call this event yourself. For C++ there is a virtual method without K2 prefix
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM|SessionModel", meta=(DisplayName = "EndSession", ScriptName = "EndSession"))
	void K2_EndSession();

	friend class UModelRepositorySubsystem;
};
