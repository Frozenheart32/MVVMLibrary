/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIView.generated.h"


class UWorldModelRepositorySubsystem;
class UModelRepositorySubsystem;
class UUIViewModel;

UENUM(BlueprintType)
enum class EUILayer : uint8
{
	BehindHUD			UMETA(DisplayName = "Behind HUD"),
	HUD					UMETA(DisplayName = "HUD"),
	GameplayView		UMETA(DisplayName = "Gameplay View"),
	PopUp,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionDelegate);

/**
 * In the paradigm, MVVM represents the base class for all widgets and windows.
 * The heirs of this class should contain only the logic of displaying information.
 * No business logic.
 */
UCLASS(Abstract)
class MVVMLIBRARY_API UUIView : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	FOnActionDelegate OnDestroyView;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MVVM|View")
	TSubclassOf<UUIViewModel> ViewModelClassType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MVVM|View")
	EUILayer ViewLayer = EUILayer::GameplayView;

private:

	UPROPERTY()
	TObjectPtr<UUIViewModel> ViewModel = nullptr;

	UPROPERTY()
	bool bIsInitializedView = false;

protected:

	virtual void NativeDestruct() override;

public:

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|View")
	EUILayer GetUILayer() const;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|View")
	bool IsInitializedView() const;


protected:

	/**
	 * Service method. Do not call from C++
	 * @param InModelRepository 
	 * @param InWorldModelRepository 
	 */
	UFUNCTION()
	void InitializeView(UModelRepositorySubsystem* InModelRepository, UWorldModelRepositorySubsystem* InWorldModelRepository);
	
	UFUNCTION(BlueprintNativeEvent, Category = "MVVM|View", meta=(ForceAsFunction))
	void ShowView();
	UFUNCTION(BlueprintNativeEvent, Category = "MVVM|View", meta=(ForceAsFunction))
	void HideView();

	friend class UWindowSubsystem;
	friend class UUIViewModel;
};
