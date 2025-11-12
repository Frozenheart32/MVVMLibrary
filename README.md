# MVVM Library Plugin

![License](https://img.shields.io/badge/license-MIT-blue) ![Version](https://img.shields.io/badge/version-1.0.0-green) ![UE5](https://img.shields.io/badge/Unreal%20Engine-5.3+-blue?logo=unrealengine)

# Purpose

The MVVM (Model-View-ViewModel) architectural pattern is designed to separate data management logic from presentation logic.

## Components

- **Model** - represents the data structure and business logic
- **View** - handles UI presentation and user interaction  
- **ViewModel** - acts as an intermediary layer between Model and View

## Data Flow

The ViewModel subscribes to:
- Data change events from the Model
- User input events from the View

When user input occurs:
1. ViewModel may update the Model data
2. Model changes trigger update events
3. ViewModel propagates these changes to the View

## Implementation Notes

- Gameplay logic interacts directly only with the Model
- The same Model can be used by multiple ViewModels simultaneously
- ViewModel is tightly coupled with its View
- When a View is destroyed, its associated ViewModel is also removed

## More info:
https://learn.microsoft.com/en-us/dotnet/architecture/maui/mvvm

# Common Class Descriptions

## 🎯 `UObjectWithWorldContext` class

### Inheritance Chain
UObject → UObjectWithWorldContext

### This class extends the capabilities of UObject by:

- Eliminating Blueprint errors related to unfilled WorldContextObject inputs
- Providing explicit UWorld access through the public `GetWorldContext()` method


# Abract classes Descriptions
## 🎯 `UUISessionModel` class

### Purpose
Serves as a base class for storing data required throughout the entire runtime.

### Inheritance Chain
UObject → UObjectWithWorldContext → UUISessionModel

### Fields

**`TWeakObjectPtr<UModelRepositorySubsystem> ModelRepository`**
Private. A weak pointer to the storage of `UUISessionModel` instances. Derived classes access this field via the `GetModelRepository` method. New values are set in the virtual `SetModelRepository` method.

### Methods

**`UModelRepositorySubsystem* GetModelRepository() const`** 
Protected. This method provides access to the session models storage.
  
**`void K2_SetModelRepository(UModelRepositorySubsystem* InModelRepository)`**
Protecated. This method is a BlueprintImplementableEvent. To retrieve other session models if needed. This event is called during the class model instance creation process.
- In **Blueprint child classes**, you can override the `SetModelRepository` event.
- In **C++ child classes**, you should override the virtual `SetModelRepository` method and also remember to call the parent logic via `Super::SetModelRepository`.

**`void K2_StartSession()`**
Protected. This method is a BlueprintImplementableEvent.
Called immediately after `K2_SetModelRepository`. Should be used when you need logic to be executed at the moment of session model instance creation.

**`void K2_EndSession()`** - This method is a BlueprintImplementableEvent.
Protected. Called in when the game shuts down for any reason.
Should be used when you need logic to be executed at the moment of session model instance destruction.

> **⚠ Warning:**
> - **Blueprint child classes** - override the events
> - **C++ child classes** - override virtual methods with the same name (without the `K2` prefix)

Virtual methods for C++ heirs:

**`virtal void SetModelRepository(UModelRepositorySubsystem* InModelRepository)`** - set parameter to ModelRepository field and call K2_SetModelRepository.

**`virtal void StartSession()`** - call K2_StartSession method.

**`virtual void EndSession()`** - call K2_EndSession method.

## 🎯 `UUIContextualModel` class

### Purpose
Serves for storing data with a lifecycle limited to a single scene (level).

### Inheritance Chain
UObject → UObjectWithWorldContext → UUIContextualModel

### Fields

**`TWeakObjectPtr<UModelRepositorySubsystem> ModelRepository`**
Private. Weak pointer to the storage of `UUISessionModel` instances. Derived classes access this field via the `GetModelRepository()` method. New values are set in the virtual `SetModelRepository()` method.

**`TWeakObjectPtr<UWorldModelRepositorySubsystem> WorldModelRepository`** 
Private. Weak pointer to the storage of `UUIContextualModel` instances. Derived classes access this field via the `GetWorldModelRepository()` method. New values are set in the virtual `SetWorldModelRepository()` method.

### Methods

**`UModelRepositorySubsystem* GetModelRepository() const`**
Protected. This method provides access to the session models storage.

**`UWorldModelRepositorySubsystem* GetWorldModelRepository() const`** 
Protected. This method provides access to the contextual models storage.

**`void K2_SetWorldModelRepository(UWorldModelRepositorySubsystem* InWorldModelRepository)`**
Protected. This method is a BlueprintImplementableEvent.
To retrieve other contextual models if needed.
This event is called during the class model instance creation process.

**`void K2_SetModelRepository(UModelRepositorySubsystem* InModelRepository)`**
Protected. This method is a BlueprintImplementableEvent.
To retrieve other session models if needed.

**`void K2_OnInitModel()`**
Protected. This method is a BlueprintImplementableEvent.
Should be used when you need logic to be executed at the moment of contextual model instance creation.

**`void K2_OnDestroyModel()`**
Protected. This method is a BlueprintImplementableEvent.
Should be used when you need logic to be executed at the moment of contextual model instance destruction.

Virtual methods for C++ heirs:

**`virtal void SetModelRepository(UModelRepositorySubsystem* InModelRepository)`** - Set value to ModelRepository field and call K2_SetModelRepository method.

**`virtal void SetWorldModelRepository(UWorldModelRepositorySubsystem* InWorldModelRepository)`** - Set value to WorldModelRepository field and call K2_SetWorldModelRepository method.

**`virtal void OnInitModel()`** - call K2_OnInitModel method.

**`virtual void OnDestroyModel()`** - call K2_OnDestroyModel method.


## 🎯 `UUIView` class

### Purpose

The View serves to display data in the user interface and may also contain input elements (e.g., purchase buttons in a store).

### Inheritance Chain
UObject -> UVisual -> UWidget -> UUserWidget -> UUIView


### Delegates

**`OnDestroyView`**
Protected. This delegate is called when the window is destroyed (upon closing or level transition). The `UIViewModel` (friend class) subscribes to this delegate in the base logic of `UIViewModel::InitializeViewModel()`.


### Fields

**`TSubclassOf<UUIViewModel> ViewModelClassType`** 
Protected. This field allows you to select the type of ViewModel that will be used for this view. Can be edited in Class Defaults.

**`EUILayer ViewLayer`** 
Protected. This field determines on which layer the view will be displayed (BehindHUD, HUD, GameplayHUD, PopUp). Can be edited in Class Defaults.

### Methods

**`void InitializeView(UModelRepositorySubsystem* InModelRepository, UWorldModelRepositorySubsystem* InWorldModelRepository)`**
Protected. This method is called from C++ in `UWindowSubsystem` (friend class). Designed to initialize the view by:
- Creating the selected UIViewModel
- Calling the following methods on the created instance:
  - `SetModelRepository`
  - `SetWorldModelRepository` 
  - `InitializeViewModel`



## 🎯 `UUIViewModel` class

## Purpose

Serves as an intermediary layer between the models and the owning `UUIView` class. This class is designed to subscribe to:
- Data change delegates in models (children of `UUISessionModel` and/or `UUIContextualModel`)
- View delegates (children of `UUIView`)

And trigger appropriate logic in response.

### Data Flow Directions

#### **Model → ViewModel → View**
Example: When a health points change delegate triggers, the `UUIView` child method updates the text field and slider fill.

#### **View → ViewModel → Model**  
Example: When a button (e.g., character ready button) is clicked, a user input delegate triggers on the `UUIView` child, and the ViewModel calls the appropriate model to execute the corresponding action.

### Inheritance Chain
UObject -> UObjectWithWorldContext -> UUIViewModel

### Fields

**`TWeakObjectPtr<UModelRepositorySubsystem> ModelRepository`**
Private. Weak pointer to the storage of `UUISessionModel` instances. Derived classes access this field via the `GetModelRepository()` method. New values are set in the virtual `SetModelRepository()` method.

**`TWeakObjectPtr<UWorldModelRepositorySubsystem> WorldModelRepository`**
Private. Weak pointer to the storage of `UUIContextualModel` instances. Derived classes access this field via the `GetWorldModelRepository()` method. New values are set in the virtual `SetWorldModelRepository()` method.

**`TWeakObjectPtr<UUIView> OwnerView`** 
Private. Weak pointer to the `UUIView` instance that owns this ViewModel.

### Methods

**`UModelRepositorySubsystem* GetModelRepository() const`** 
Protected. This method provides access to the session models storage.

**`UWorldModelRepositorySubsystem* GetWorldModelRepository() const`**
Protected. This method provides access to the contextual models storage.

**`APlayerController* GetOwningPlayer() const`**
Protected. This method returns a pointer to the owning player controller instance. It will use the controller specified during window creation, or attempt to retrieve player controller at index 0 from UWorld.

**`UWindowSubsystem* GetWindowSubsystem() const`** 
Protected. This method returns a pointer to the subsystem for opening/closing other windows.

**`UUIView* GetOwnerView() const`** 
Protected. This method returns a pointer to the view that owns this ViewModel.


**`void K2_SetWorldModelRepository(UWorldModelRepositorySubsystem* InWorldModelRepository)`** 
Protected. (UUView - friend class). This method is a BlueprintImplementableEvent.
To retrieve other contextual models if needed.
This event is called during the ViewModel class instance creation process.

**`void K2_SetModelRepository(UModelRepositorySubsystem* InModelRepository)`**
Protected. (UUView - friend class). This method is a BlueprintImplementableEvent.
To retrieve other session models if needed.
This event is called during the ViewModel class instance creation process.

**`void K2_InitializeViewModel(UUIView* View)`** 
Protected. (UUView - friend class). This method is a BlueprintImplementableEvent.
Called immediately after `K2_SetModelRepository`. Should be used to initialize the owning view (`UUIView` child) with initial values and subscribe to model and/or view events.

**`void K2_OnDestroyViewModel(UUIView* View)`**
Protected. This method is a BlueprintImplementableEvent. Called when the `OnDestroyView` delegate is broadcasted on the `OwnerView`. In the base logic, it unsubscribes from this event. You can extend this logic to unsubscribe from your custom subscriptions to model and view delegate.


Virtual methods for C++ heirs:

**`virtal void SetWorldModelRepository(UWorldModelRepositorySubsystem* InWorldModelRepository)`** Set parameter to WorldModelRepository field and call K2_SetWorldModelRepository method.

**`virtal void SetModelRepository(UModelRepositorySubsystem* InModelRepository)`** Set parameter to ModelRepository field and call K2_SetModelRepository method.

**`virtual void InitializeViewModel(UUIView* View)`** - set parameter to OwningView field. Subscribing on OnDestroyView delegate (UUIVIew). Call K2_InitializeViewModel method.

**`virtual void OnDestroyViewModel()`** - Unsubscribing from OnDestroyView delegate (UUIVIew). Call K2_OnDestroyViewModel method.


## 🎯 `UUIPopUpView` class

## Purpose

`PopUpView` serves to display data in a pop-up user interface. The key feature is that `PopUp` operates without a ViewModel and receives `ModelRepository` and `WorldRepository` directly. It also includes auto-removal logic after a specified time (disabled by default).

As `UUIView`, it is instantiated through `UWindowSubsystem`.

### Inheritance Chain
UObject -> UVisual -> UWidget -> UUserWidget -> UUIPopUpView

### Delegates

**`OnDestroyView`**
Public. This delegate is called when the PopUp is destroyed (timer or closing).


### Fields

**`EUILayer ViewLayer`** 
Protected. This field determines on which layer the view will be displayed (BehindHUD, HUD, GameplayHUD, PopUp). Can be edited in Class Defaults.


**`bool bUseSelfDestroyTimer`** 
Protected. Determines whether the self-destruction timer will be used for the PopUp.

**`float LifeSpan`** 
Protected. Default value = 3.f. Self destroy timer.

### Methods
**`void K2_InitializePopUp(UModelRepositorySubsystem* InModelRepository, UWorldModelRepositorySubsystem* InWorldModelRepository)`**
Protected. 

**`EUILayer GetUILayer() const`** 
Public. Returns information about which layer the PopUp operates on.

**`bool IsInitializedPopUp() const`** 
Public. Checks whether the PopUp initialization has been completed.

**`UModelRepositorySubsystem* GetModelRepository() const`**
Public. Provides access to session models.

**`UWorldModelRepositorySubsystem* GetWorldModelRepository() const`**
Public. Provides access to contextual models.



# Storage Class Descriptions

## 🎯 `UModelRepositorySubsystem` class

## Purpose

This class is used to store session models, i.e. models that live during all runtime.

### Inheritance Chain
UObject -> USubsystem -> UGameInstanceSubsystem -> UModelRepositorySubsystem

### Fields

**`TMap<UClass*, UUISessionModel*> SessionModels`**
Private. This field stores created session models.

### Methods

**`UUISessionModel* GetSessionModel(TSubclassOf<UUISessionModel> ModelType)`** 
Public. This C++ method attempts to find a model of the requested type in the `SessionModels` container. If found, it returns immediately; if not, it creates one via the `CreateSessionModel` method which returns a reference to the new model object.

**`T* GetSessionModel<T>(TSubclassOf<UUISessionModel> ModelType)`** 
Public. This template method for C++ calls the previous method and performs `Cast<T>` on its result.

**`void K2_GetSessionModel(UUISessionModel*& OutSessionModel, TSubclassOf<UUISessionModel> ModelType)`** 
Protected. This method variant is for Blueprints only.

**`void CloseSession()`**
Public. This C++ method should only be called in `GameInstance` on the `Shutdown` event. It triggers the `EndSession` event on all stored models.

**`UUISessionModel* CreateSessionModel(const TSubclassOf<UUISessionModel>& ModelType)`** 
Private. This method creates an instance of a session model of the specified type and performs the following actions in order:
- Adds the instance to the `SessionModels` container
- Calls `K2_SetModelRepository`
- Calls `K2_StartSession`


## 🎯 `UWorldModelRepositorySubsystem` class

## Purpose

This class is used to store contextual models, i.e. models that live during one scene (level).

### Inheritance Chain
UObject -> USubsystem -> UWorldSubsystem  -> UWorldModelRepositorySubsystem

### Fields

**`TMap<UClass*, UUIContextualModel*> ContextualModels`** 
Private. This field stores created contextual models.

**`TWeakObjectPtr<UModelRepositorySubsystem> ModelRepositorySubsystemCache`**
Private. Cached ModelRepository pointer.

### Methods
**`UUIContextualModel* GetContextualModel(TSubclassOf<UUIContextualModel> ModelType)`** 
Public. This C++ method attempts to find a model of the requested type in the `ContextualModels` container. If found, it returns immediately; if not, it creates one via the `CreateContextualModel` method which returns a reference to the new model object.

**`T* GetContextualModel<T>(TSubclassOf<UUIContextualModel> ModelType)`** 
Public. This template method for C++ calls the previous method and performs `Cast<T>` on its result.

**`void K2_GetContextualModel(UUIContextualModel*& OutContextualModel, TSubclassOf<UUIContextualModel> ModelType)`** 
Protected. This method variant is for Blueprints only.

**`UUIContextualModel* CreateContextualModel(const TSubclassOf<UUIContextualModel>& ModelType)`** 
Private. This method creates an instance of a contextual model of the specified type and performs the following actions in order:
- Adds the instance to the `ContextualModels` container
- Calls `K2_SetModelRepository`
- Calls `K2_SetWorldModelRepository`
- Calls `K2_OnInitModel`


## 🎯 `UWindowSubsystem` class

## Purpose

Serves for managing the creation and destruction of `UUIView` and `UUIPopUpView` instances. Lifetime - single scene (level).

### Fields

**`TMap<UClass*, UUIView*> OpenedWindows`** 
Private. This field stores currently opened `UUIView` instances.

### Methods

**`UUIView* OpenWindow(TSubclassOf<UUIView> WindowType, APlayerController* Owner = nullptr)`**  
Public. This C++ method creates a new window if one of the requested type doesn't already exist in `OpenedWindows`. It performs the following actions on the new instance:
- Calls `AddToViewport` on it, converting the enum to int as a parameter for layer selection
- Calls `InitializeView` on it
- Adds the new window to `OpenedWindows`

**`T* OpenWindow<T>(TSubclassOf<UUIView> WindowType, APlayerController* Owner = nullptr)`** 
Public. This template method for C++ calls the previous method and performs `Cast<T>` on its result.

**`void K2_OpenWindow(UUIView*& OutWindow, bool& bResult, TSubclassOf<UUIView> WindowType, APlayerController* Owner = nullptr)`** 
Protected. This method variant is for Blueprints only.

**`UUIView* CreateWindow(const TSubclassOf<UUIView>& WindowType, APlayerController* Owner) const`** 
Private. This method creates an instance of `UIView` of the specified type.

**`bool CloseWindow(TSubclassOf<UUIView> WindowType)`** 
Public. Closes the window of the specified type if it was open.

**`void CloseAllWindows()`** 
Public. Closes all open windows.

**`bool IsOpen(TSubclassOf<UUIView> WindowType) const`**
Public. Returns whether a window of the specified type is currently open.


**`UUIPopUpView* CreatePopUp(TSubclassOf<UUIPopUpView> PopUpType, APlayerController* Owner = nullptr, UPanelWidget* ParentWidget = nullptr)`**  
Public. This C++ method creates a new PopUp. It performs the following actions on the new instance:
- Calls `AddToViewport` on it, converting the enum to int as a parameter for layer selection
- Calls `InitializePopUp` on it.

**`T* CreatePopUp(TSubclassOf<UUIPopUpView> PopUpType, APlayerController* Owner = nullptr, UPanelWidget* ParentWidget = nullptr)`** 
Public. This template method for C++ calls the previous method and performs `Cast<T>` on its result.

**`void K2_CreatePopUp(UUIPopUpView*& OutPopUp, bool& bResult, UPARAM(meta=(AllowAbstract=false))TSubclassOf<UUIPopUpView> PopUpType, APlayerController* Owner = nullptr, UPanelWidget* ParentWidget = nullptr)`** 
Protected. This method variant is for Blueprints only.

