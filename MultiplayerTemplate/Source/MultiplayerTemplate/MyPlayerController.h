// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"
struct FInputActionValue;
// DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();
	virtual void AcknowledgePossession(class APawn* P) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	void OnMove(const FInputActionValue& Value);
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
};
