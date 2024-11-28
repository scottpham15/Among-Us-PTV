// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapSubsystem.h"

void UMinimapSubsystem::RegisterMainCharacterEntry(AActor* Target, bool bIsStatic, UTexture2D* Icon)
{
	MainEntry = FMiniEntry(Target, bIsStatic, Icon);
	OnMainCharacterRegister.Broadcast(MainEntry);
}

void UMinimapSubsystem::RegisterEntry(AActor* Target, bool bIsStatic, UTexture2D* Icon)
{
	FMiniEntry NewEntry = FMiniEntry(Target, bIsStatic, Icon);
	Entries.Add(NewEntry);
	OnNewEntryRegister.Broadcast(NewEntry);
}

void UMinimapSubsystem::UnregisterEntry(AActor* Target)
{
	int IndexToRemove = -1;
	for (int i = 0; i < Entries.Num(); i++)
	{
		if (Entries[i].Actor == Target)
		{
			IndexToRemove = i;
			break;
		}
	}

	if (IndexToRemove >= 0)
	{
		OnEntryRemove.Broadcast(Entries[IndexToRemove].Actor);
		Entries.RemoveAt(IndexToRemove);
	}
}
