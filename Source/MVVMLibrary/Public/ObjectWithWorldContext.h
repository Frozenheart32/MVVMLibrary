/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ObjectWithWorldContext.generated.h"

class UWorld;

/**
 * Allows an UObject to use methods and subsystems through the world context.
 */
UCLASS(BlueprintType, Blueprintable)
class MVVMLIBRARY_API UObjectWithWorldContext : public UObject
{
	GENERATED_BODY()

private:
	
	UPROPERTY()
	mutable TWeakObjectPtr<UWorld> CachedWorld = nullptr;

public:
	
	UFUNCTION(BlueprintCallable, Category = "UObject|WorldContext")
	UWorld* GetWorldContext() const;

	virtual UWorld* GetWorld() const override;
};
