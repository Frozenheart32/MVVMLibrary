/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ModelRepositorySubsystem.generated.h"

class UUISessionModel;

/**
 * This class is used to store session models, i.e. models that live during all runtime.
 */
UCLASS(NotBlueprintable, BlueprintType)
class MVVMLIBRARY_API UModelRepositorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	
	UPROPERTY()
	TMap<UClass*, UUISessionModel*> SessionModels;

protected:
	/**
	 * Blueprint variant GetSessionModel. 
	 * @param OutSessionModel Selected model
	 * @param ModelType Selected model type
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, DisplayName = "GetSessionModel", Category = "MVVM|ModelRepository", meta=(DeterminesOutputType = "ModelType", DynamicOutputParam = "OutSessionModel"))
	void K2_GetSessionModel(UUISessionModel*& OutSessionModel, UPARAM(meta=(AllowAbstract=false))TSubclassOf<UUISessionModel> ModelType);

public:
	/**
	 * C++ variant GetSessionModel. 
	 * @param ModelType Selected model type
	 * @return Selected model
	 */
	UFUNCTION()
	UUISessionModel* GetSessionModel(TSubclassOf<UUISessionModel> ModelType);

	/**
	 * C++ template variant of GetSessionModel method. Auto Cast to T type
	 * @tparam T Inheritor of UUISessionModel class
	 * @return Selected model
	 */
	template<class T = UUISessionModel>
	T* GetSessionModel()
	{
		static_assert(TIsDerivedFrom<T, UUISessionModel>::IsDerived, "GetSessionModel can only be used to create UUISessionModel instances");

		return Cast<T>(GetSessionModel(T::StaticClass()));
	}

	/**
	 * Calls the native End Session event on all session models. Should only be called in a Game instance on Shutdown event.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|ModelRepository")
	void CloseSession();

private:
	
	UFUNCTION()
	UUISessionModel* CreateSessionModel(const TSubclassOf<UUISessionModel>& ModelType);
};
