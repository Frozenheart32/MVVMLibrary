/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WindowSubsystem.generated.h"

class UUIPopUpView;
class UWorldModelRepositorySubsystem;
class APlayerController;
class UModelRepositorySubsystem;
class UUIView;
class UPanelWidget;

/**
 * Serves for spawning, storing and closing windows. Life cycle is one scene
 */
UCLASS(NotBlueprintable, BlueprintType)
class MVVMLIBRARY_API UWindowSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TMap<UClass*, UUIView*> OpenedWindows;

	UPROPERTY(BlueprintReadOnly, Category = "MVVM|WindowSubsystem", meta=(AllowPrivateAccess))
	bool bIsHiddenAllWindows = false;

protected:

	/**
	 * Blueprint variant of OpenWindow method
	 * @param WindowType Selected window type
	 * @param Owner if nullptr == Owner is WindowService
	 * @param bResult Is Successful?
	 * @param OutWindow Opened Window
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, DisplayName = "OpenWindow", Category = "MVVM|WindowSubsystem", meta=(DeterminesOutputType = "WindowType", DynamicOutputParam = "OutWindow", ExpandBoolAsExecs="bResult"))
	void K2_OpenWindow(UUIView*& OutWindow, bool& bResult, UPARAM(meta=(AllowAbstract=false))TSubclassOf<UUIView> WindowType, APlayerController* Owner = nullptr);

	/**
	 * Blueprint variant of OpenWindow method
	 * @param PopUpType Selected Pop-Up type
	 * @param Owner if nullptr == Owner is WindowService
	 * @param ParentWidget Content Widget
	 * @param bResult Is Successful?
	 * @param OutPopUp Created Pop-up
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, DisplayName = "CreatePopUp", Category = "MVVM|WindowSubsystem", meta=(DeterminesOutputType = "PopUpType", DynamicOutputParam = "OutPopUp", ExpandBoolAsExecs="bResult"))
	void K2_CreatePopUp(UUIPopUpView*& OutPopUp, bool& bResult, UPARAM(meta=(AllowAbstract=false))TSubclassOf<UUIPopUpView> PopUpType, APlayerController* Owner = nullptr, UPanelWidget* ParentWidget = nullptr);

public:

	/**
	 * C++ variant of OpenWindow method
	 * @param WindowType Selected window type
	 * @param Owner - nullptr == Owner is WindowService
	 * @return Needed Window
	 */
	UFUNCTION()
	UUIView* OpenWindow(TSubclassOf<UUIView> WindowType, APlayerController* Owner = nullptr);

	/**
	 * C++ template variant of OpenWindow method. Auto Cast to T type
	 * @tparam T Inheritor of UUIView class
	 * @param Owner nullptr == Owner is WindowService
	 * @return Needed Pop-up widget
	 */
	template<class T = UUIView>
	T* OpenWindow(TSubclassOf<UUIView> WindowType, APlayerController* Owner = nullptr)
	{
		static_assert(TIsDerivedFrom<T, UUIView>::IsDerived, "OpenWindow can only be used to create UUIView instances");
		
		check(IsValid(WindowType) && (WindowType->IsChildOf(T::StaticClass()) || WindowType == T::StaticClass()));
		return Cast<T>(OpenWindow(MoveTemp(WindowType), Owner));
	}
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	bool CloseWindow(TSubclassOf<UUIView> WindowType);
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	void CloseAllWindows();
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	bool IsOpen(TSubclassOf<UUIView> WindowType) const;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	void InitializeExistsView(UUIView* ExistedView);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	void HideAllWindows();
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "MVVM|WindowSubsystem")
	void ShowAllWindows();

	/**
	 * C++ variant of CreatePopUp method
	 * @param PopUpType Selected Pop-up type
	 * @param Owner - nullptr == Owner is WindowService
	 * @param ParentWidget Content widget
	 * @return Needed Pop-up widget
	 */
	UFUNCTION()
	UUIPopUpView* CreatePopUp(TSubclassOf<UUIPopUpView> PopUpType, APlayerController* Owner = nullptr, UPanelWidget* ParentWidget = nullptr);

	/**
	 * C++ template variant of CreatePopUp method. Auto Cast to T type
	 * @tparam T Inheritor of UUIPopUpView class
	 * @param Owner nullptr == Owner is WindowService
	 * @param ParentWidget Content widget
	 * @return Needed Pop-up widget
	 */
	template<class T = UUIPopUpView>
	T* CreatePopUp(TSubclassOf<UUIPopUpView> PopUpType, APlayerController* Owner = nullptr, UPanelWidget* ParentWidget = nullptr)
	{
		static_assert(TIsDerivedFrom<T, UUIPopUpView>::IsDerived, "CreatePopUp can only be used to create UUIPopUpView instances");
		
		check(IsValid(PopUpType) && (PopUpType->IsChildOf(T::StaticClass()) || PopUpType == T::StaticClass()));
		
		return Cast<T>(CreatePopUp(MoveTemp(PopUpType), Owner, ParentWidget));
	}

private:

	UUIView* CreateWindow(const TSubclassOf<UUIView>& WindowType, APlayerController* Owner) const;
};
