/*
* Copyright (c) 2025 Alexsander Khrapin
* Licensed under the MIT License. See LICENSE in the project root for license information.
*/


#include "ObjectWithWorldContext.h"

UWorld* UObjectWithWorldContext::GetWorldContext() const
{
	return GetWorld();
}

UWorld* UObjectWithWorldContext::GetWorld() const
{
	if (CachedWorld.IsValid())
	{
		return CachedWorld.Get();
	}

	// Could be a GameInstance, could be World, could also be a WidgetTree, so we're just going to follow
	// the outer chain to find the world we're in.
	const UObject* Outer = GetOuter();
	while (Outer)
	{
		if (UWorld* World = Outer->GetWorld())
		{
			CachedWorld = World;
			return World;
		}

		Outer = Outer->GetOuter();
	}

	return nullptr;
}
