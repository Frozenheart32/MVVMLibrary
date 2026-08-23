/*
* Copyright (c) 2026 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UIPopupView.generated.h"

class UWindowSubsystem;
/**
 * 
 */
UCLASS(Abstract)
class MVVMLIBRARY_API UUIPopupView : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MVVM|Pop-Up")
	bool bUseSelfDestroyTimer = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MVVM|Pop-Up")
	float LifeSpan = 3.f;

private:

	UPROPERTY()
	bool bIsStartedPopup = false;

	UPROPERTY()
	TWeakObjectPtr<UModelRepositorySubsystem> ModelRepository;
	UPROPERTY()
	TWeakObjectPtr<UWorldModelRepositorySubsystem> WorldModelRepository;

	UPROPERTY()
	FTimerHandle SelfDestroyTimerHandle;

protected:

	/**
	 * Calling from lambda (WindowSubsystem)
	 * @param InModelRepository
	 * @param InWorldModelRepository 
	 */
	UFUNCTION()
	void InitializePopup(UModelRepositorySubsystem* InModelRepository, UWorldModelRepositorySubsystem* InWorldModelRepository);
	UFUNCTION(BlueprintNativeEvent, Category = "MVVM|Pop-Up", meta=(ForceAsFunction))
	void FeedPopupData(UObject* FeedDataObject);


	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

public:

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|Pop-Up")
	bool IsStartedPopUp() const;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|Pop-Up")
	UModelRepositorySubsystem* GetModelRepository() const;
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|Pop-Up")
	UWorldModelRepositorySubsystem* GetWorldModelRepository() const;

protected:
	
	friend class UWindowSubsystem;

private:
	
	UFUNCTION()
	void StartDestroyLogic();

	UFUNCTION()
	void OnDestroyTimerComplete();
};
