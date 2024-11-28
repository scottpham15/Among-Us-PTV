// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AMyGameMode::AMyGameMode()
{
	
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	PlayerList.Add(NewPlayer);
}
