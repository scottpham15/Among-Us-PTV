// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyCharacter.h"

class UEnhancedInputLocalPlayerSubsystem;

AMyPlayerController::AMyPlayerController(): DefaultMappingContext(nullptr), MoveAction(nullptr)
{
	bShowMouseCursor = true;
	bReplicates = true;
}

void AMyPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
	if (AMyCharacter* CharacterBase = Cast<AMyCharacter>(P))
	{
		CharacterBase->SetupACS();
	}
}


void AMyPlayerController::OnMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetPawn()->AddMovementInput(RightDirection, MovementVector.X);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMyPlayerController::OnMove);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::OnMove);
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}