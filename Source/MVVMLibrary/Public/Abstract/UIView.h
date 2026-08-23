/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "WorldModelRepositorySubsystem.h"
#include "UIView.generated.h"


class UWorldModelRepositorySubsystem;
class UModelRepositorySubsystem;
class UUIViewModel;

DECLARE_MULTICAST_DELEGATE(FOnViewActionDelegate);

/**
 * In the paradigm, MVVM represents the base class for all widgets and windows.
 * The heirs of this class should contain only the logic of displaying information.
 * No business logic.
 */
UCLASS(Abstract)
class MVVMLIBRARY_API UUIView : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:

	FOnViewActionDelegate OnActivatedView;
	FOnViewActionDelegate OnDeactivatedView;
	
	FOnViewActionDelegate OnDestroyView;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MVVM|View")
	TSubclassOf<UUIViewModel> ViewModelClassType;

private:

	UPROPERTY()
	TObjectPtr<UUIViewModel> ViewModel = nullptr;

	UPROPERTY()
	bool bIsInitializedView = false;

protected:
	
	/**
	 * Calling from lambda (WindowSubsystem)
	 * @param InModelRepository 
	 * @param InWorldModelRepository 
	 */
	UFUNCTION()
	void InitializeView(UModelRepositorySubsystem* InModelRepository, UWorldModelRepositorySubsystem* InWorldModelRepository);
	
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
	virtual void NativeDestruct() override;

public:

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|View")
	bool IsInitializedView() const;

protected:
	
	
	UFUNCTION(BlueprintNativeEvent, Category = "MVVM|View", meta=(ForceAsFunction))
	void ShowView();
	UFUNCTION(BlueprintNativeEvent, Category = "MVVM|View", meta=(ForceAsFunction))
	void HideView();

	friend class UWindowSubsystem;
	friend class UUIViewModel;
	
};
