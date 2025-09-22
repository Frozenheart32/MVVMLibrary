/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "UIView.h"
#include "UIPopUpView.generated.h"

class UWindowSubsystem;
/**
 * 
 */
UCLASS(Abstract)
class MVVMLIBRARY_API UUIPopUpView : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnActionDelegate OnDestroyPopUp;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MVVM|Pop-Up")
	EUILayer ViewLayer = EUILayer::PopUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MVVM|Pop-Up")
	bool bUseSelfDestroyTimer = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MVVM|Pop-Up")
	float LifeSpan = 3.f;

private:

	UPROPERTY()
	bool bIsInitializedPopUp = false;

	UPROPERTY()
	TWeakObjectPtr<UModelRepositorySubsystem> ModelRepository;
	UPROPERTY()
	TWeakObjectPtr<UWorldModelRepositorySubsystem> WorldModelRepository;

	UPROPERTY()
	FTimerHandle SelfDestroyTimerHandle;

protected:

	virtual void NativeDestruct() override;

	/**
	 * Should be overridden in C++ heirs
	 * @param InModelRepository - store of Session Models
	 * @param InWorldModelRepository - store of Contextual Models
	 */
	UFUNCTION()
	virtual void InitializePopUp(UModelRepositorySubsystem* InModelRepository, UWorldModelRepositorySubsystem* InWorldModelRepository);

public:
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|Pop-Up")
	EUILayer GetUILayer() const;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|Pop-Up")
	bool IsInitializedPopUp() const;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|Pop-Up")
	UModelRepositorySubsystem* GetModelRepository() const;
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|Pop-Up")
	UWorldModelRepositorySubsystem* GetWorldModelRepository() const;

protected:

	/**
	 * Service method. Do not call from C++
	 * @param InModelRepository 
	 * @param InWorldModelRepository 
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "MVVM|Pop-Up", meta=(ForceAsFunction, DisplayName = "InitializePopUp", ScriptName = "InitializePopUp"))
	void K2_InitializePopUp(UModelRepositorySubsystem* InModelRepository, UWorldModelRepositorySubsystem* InWorldModelRepository);

	friend class UWindowSubsystem;

private:

	UFUNCTION()
	void OnDestroyTimerComplete();
};
