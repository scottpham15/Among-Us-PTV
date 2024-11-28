// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyGameMode();

	UPROPERTY(BlueprintReadOnly)
	TArray<AController*> PlayerList;

	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPostLogin(AController* NewPlayer) override;
};
