/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "WindowSubsystem.h"

#include "Abstract/UIView.h"
#include "WorldModelRepositorySubsystem.h"
#include "ModelRepositorySubsystem.h"
#include "Abstract/UIPopUpView.h"
#include "Components/PanelWidget.h"


void UWindowSubsystem::K2_OpenWindow(UUIView*& OutWindow, bool& bResult, TSubclassOf<UUIView> WindowType,
                                     APlayerController* Owner)
{
	OutWindow = OpenWindow(MoveTemp(WindowType), Owner);
	bResult = OutWindow != nullptr;
}

void UWindowSubsystem::K2_CreatePopUp(UUIPopUpView*& OutPopUp, bool& bResult, TSubclassOf<UUIPopUpView> PopUpType,
	APlayerController* Owner, UPanelWidget* ParentWidget)
{
	OutPopUp = CreatePopUp(MoveTemp(PopUpType), Owner, ParentWidget);
	bResult = OutPopUp != nullptr;
}

UUIView* UWindowSubsystem::OpenWindow(TSubclassOf<UUIView> WindowType, APlayerController* Owner)
{
	if(IsRunningDedicatedServer() || !GetWorld() || !IsValid(WindowType)) return nullptr;

	if(OpenedWindows.Contains(WindowType))
	{
		return OpenedWindows[WindowType];
	}
	
	const auto Window = CreateWindow(WindowType, Owner);
	OpenedWindows.Add(WindowType, Window);

	if(bIsHiddenAllWindows)
	{
		Window->K2_HideView();
	}
	
	Window->AddToViewport(static_cast<int32>(Window->GetUILayer()));
	InitializeExistsView(Window);

	return Window;
}

bool UWindowSubsystem::CloseWindow(TSubclassOf<UUIView> WindowType)
{
	if(IsRunningDedicatedServer()) return false;
	
	if(OpenedWindows.Contains(WindowType))
	{
		if(const auto FindWindow = OpenedWindows[WindowType])
			FindWindow->RemoveFromParent();
		
		OpenedWindows.Remove(WindowType);
		return true;
	}

	return false;
}

void UWindowSubsystem::CloseAllWindows()
{
	if(IsRunningDedicatedServer()) return;
	
	for (const auto& [WindowType, Window] : OpenedWindows)
	{
		if(Window)
		{
			Window->RemoveFromParent();
		}
		
		OpenedWindows.Remove(WindowType);
	} 
}

bool UWindowSubsystem::IsOpen(TSubclassOf<UUIView> WindowType) const
{
	if(!IsValid(WindowType))
		return false;
	
	return OpenedWindows.Contains(WindowType);
}

void UWindowSubsystem::InitializeExistsView(UUIView* ExistedView)
{
	if(IsRunningDedicatedServer() || !ExistedView || ExistedView->IsInitializedView()) return;

	const auto WorldModelRepositorySubsystem = GetWorld()->GetSubsystem<UWorldModelRepositorySubsystem>();
	const auto ModelRepositorySubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UModelRepositorySubsystem>();
	check(ModelRepositorySubsystem && WorldModelRepositorySubsystem);

	ExistedView->InitializeView(ModelRepositorySubsystem, WorldModelRepositorySubsystem);
}

void UWindowSubsystem::HideAllWindows()
{
	if(bIsHiddenAllWindows) return;
	
	bIsHiddenAllWindows = true;
	for (const auto& WindowPair : OpenedWindows)
	{
		WindowPair.Value->K2_HideView();
	} 
}

void UWindowSubsystem::ShowAllWindows()
{
	if(!bIsHiddenAllWindows) return;
	
	bIsHiddenAllWindows = false;
	for (const auto& WindowPair : OpenedWindows)
	{
		WindowPair.Value->K2_ShowView();
	}
}

UUIPopUpView* UWindowSubsystem::CreatePopUp(TSubclassOf<UUIPopUpView> PopUpType, APlayerController* Owner, UPanelWidget* ParentWidget)
{
	if(IsRunningDedicatedServer() || !GetWorld() || !IsValid(PopUpType)) return nullptr;

	const auto NameText = PopUpType->GetName();
	UUIPopUpView* PopUp = nullptr;
	if(IsValid(Owner))
	{
		PopUp = Cast<UUIPopUpView>(CreateWidget(Owner, PopUpType, FName{NameText}));
	}
	else
	{
		PopUp = Cast<UUIPopUpView>(CreateWidget(GetWorld(), PopUpType, FName{NameText}));
	}
	
	if(IsValid(ParentWidget))
	{
		ParentWidget->AddChild(PopUp);
	}
	else
	{
		PopUp->AddToViewport(static_cast<int32>(PopUp->GetUILayer()));
	}

	const auto WorldModelRepositorySubsystem = GetWorld()->GetSubsystem<UWorldModelRepositorySubsystem>();
	const auto ModelRepositorySubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UModelRepositorySubsystem>();
	PopUp->K2_InitializePopUp(ModelRepositorySubsystem, WorldModelRepositorySubsystem);

	return PopUp;
}

UUIView* UWindowSubsystem::CreateWindow(const TSubclassOf<UUIView>& WindowType, APlayerController* Owner) const
{
	const auto NameText = WindowType->GetName();
	if(IsValid(Owner))
	{
		return Cast<UUIView>(CreateWidget(Owner, WindowType, FName{NameText}));
	}
	else
	{
		return Cast<UUIView>(CreateWidget(GetWorld(), WindowType, FName{NameText}));
	}
}
