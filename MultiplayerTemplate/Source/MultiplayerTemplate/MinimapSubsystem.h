// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MinimapSubsystem.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FMiniEntry
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	AActor* Actor;

	UPROPERTY(BlueprintReadOnly)
	bool bIsStatic;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Icon;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewMinimapEntry, FMiniEntry, Entry);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMinimapEntryRemove, AActor*, Entry);

UCLASS()
class MULTIPLAYERTEMPLATE_API UMinimapSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FMiniEntry> Entries;

	FMiniEntry MainEntry;

	UFUNCTION(BlueprintCallable)
	void RegisterMainCharacterEntry(AActor* Target, bool bIsStatic, UTexture2D* Icon);
	
	UFUNCTION(BlueprintCallable)
	void RegisterEntry(AActor* Target, bool bIsStatic, UTexture2D* Icon);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FMiniEntry> GetAllCurrentEntries() const
	{
		return Entries;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FMiniEntry GetMainCharacterEntry() const
	{
		return MainEntry;
	}

	UFUNCTION(BlueprintCallable)
	void UnregisterEntry(AActor* Target);

	UPROPERTY(BlueprintAssignable)
	FNewMinimapEntry OnNewEntryRegister;

	UPROPERTY(BlueprintAssignable)
	FNewMinimapEntry OnMainCharacterRegister;
	
	UPROPERTY(BlueprintAssignable)
	FMinimapEntryRemove OnEntryRemove;
	
};
