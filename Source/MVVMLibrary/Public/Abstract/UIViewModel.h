/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "ObjectWithWorldContext.h"
#include "UIViewModel.generated.h"

class UWorldModelRepositorySubsystem;
class UWindowSubsystem;
class UModelRepositorySubsystem;
class UUIView;

/**
 * In the MVVM paradigm, it represents a base class for creating a layer between widgets and data.
 * The heirs of this class should contain logic that tracks changes in data in models,
 * logic for responding to user input events from the view,
 * and logic for calling data updates on the view.
 */
UCLASS(Blueprintable, BlueprintType)
class MVVMLIBRARY_API UUIViewModel : public UObjectWithWorldContext
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TWeakObjectPtr<UUIView> OwnerView = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UModelRepositorySubsystem> ModelRepository = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UWorldModelRepositorySubsystem> WorldModelRepository = nullptr;

protected:
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(BlueprintProtected), Category = "MVVM|ViewModel")
	UModelRepositorySubsystem* GetModelRepository() const;
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(BlueprintProtected), Category = "MVVM|ViewModel")
	UWorldModelRepositorySubsystem* GetWorldModelRepository() const;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(BlueprintProtected), Category = "MVVM|ViewModel")
	UWindowSubsystem* GetWindowSubsystem() const;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(BlueprintProtected), Category = "MVVM|ViewModel")
	APlayerController* GetOwningPlayer() const;

	/**
	 * Gives the view to which the viewmodel belongs
	 * @return View
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(BlueprintProtected), Category = "MVVM|ViewModel")
	UUIView* GetOwnerView() const;
	
	/**
	 * Event calling when UView delegate OnDestroyView is broadcasted. You can use an override to unsubscribe own delegate bindings.
	 */
	UFUNCTION()
	virtual void OnDestroyViewModel();
	
	/**
	 * Should be overridden in C++ heirs
	 * @param InModelRepository - store of Session Models
	 */
	UFUNCTION()
	virtual void SetModelRepository(UModelRepositorySubsystem* InModelRepository);
	
	/**
	 * Should be overridden in C++ heirs
	 * @param InWorldModelRepository - store of Contextual Models
	 */
	UFUNCTION()
	virtual void SetWorldModelRepository(UWorldModelRepositorySubsystem* InWorldModelRepository);

	/**
	 * Should be overridden in C++ heirs
	 * @param View - Owning View
	 */
	UFUNCTION()
	virtual void InitializeViewModel(UUIView* View);

protected:

	/**
	 * Event calling when UView delegate OnDestroyView is broadcasted. You can use an override to unsubscribe own delegate bindings.
	 * Need calling parent version.
	 * Do not call this event yourself. For C++ there is a virtual method without K2 prefix
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM|ViewModel", meta=(ForceAsFunction, DisplayName = "OnDestroyViewModel", ScriptName = "OnDestroyViewModel"))
	void K2_OnDestroyViewModel();
	
	/**
	 * Sets the model repository. You can use an override to get the required dependencies.
	 * Do not call this event yourself. For C++ there is a virtual method without K2 prefix
	 * @param InModelRepository 
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM|ViewModel", meta=(ForceAsFunction, DisplayName = "SetModelRepository", ScriptName = "SetModelRepository"))
	void K2_SetModelRepository(UModelRepositorySubsystem* InModelRepository);

	/**
	 * Sets the world model repository. You can use an override to get the required dependencies.
	 * Do not call this event yourself. For C++ there is a virtual method without K2 prefix
	 * @param InWorldModelRepository 
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM|ViewModel", meta=(ForceAsFunction, DisplayName = "SetWorldModelRepository", ScriptName = "SetWorldModelRepository"))
	void K2_SetWorldModelRepository(UWorldModelRepositorySubsystem* InWorldModelRepository);

	/**
	 * Base implementation. Serves to cast a view reference to the desired type, subscriptions to view and model delegates.
	 * Need calling parent version!
	 * Do not call this event yourself. For C++ there is a virtual method without K2 prefix
	 * @param View 
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM|ViewModel", meta=(ForceAsFunction, DisplayName = "InitializeViewModel", ScriptName = "InitializeViewModel"))
	void K2_InitializeViewModel(UUIView* View);
	
	friend class UUIView;
};
