// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "AbilitySystemComponent.h"
#include "MyAttributeSet.h"
#include "MyPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	IsGhost = false;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
// ue 5.4.4
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	BasicAttributeSet = CreateDefaultSubobject<UMyAttributeSet>(TEXT("BasicAttributeSet"));
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent)
	{
		SetupACS();
		BasicAttributeSet->InitHealth(100);
	}

	// ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
	SetOwner(NewController);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::SetupACS()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BasicAttributeSet->GetHealthAttribute()).AddUObject(this, &AMyCharacter::OnHealthChange);
}

void AMyCharacter::DeadBodyReported(ADeadBody* DeadBodyDes)
{
	FoundDeadBody.Broadcast(DeadBodyDes);
}

void AMyCharacter::AwayFromTheDeadBody()
{
	AwayFromDeadBody.Broadcast();
}

void AMyCharacter::OnRep_IsDead()
{
	if (!IsValid(GetController())) // ai day khac bien thanh ma
	{
		APlayerController* LocalController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0);
		const bool IsLocalControllerValid = IsValid(LocalController);
		bool IsLocalGhost = false;
		if (IsLocalControllerValid && IsValid(LocalController->GetPawn()))
			IsLocalGhost = Cast<AMyCharacter>(LocalController->GetPawn())->IsGhost;
		if (IsLocalControllerValid && IsLocalGhost)
		{
			SetActorHiddenInGame(false); // se hien hinh
		}
		else
		{
			SetActorHiddenInGame(true); // se bi tang hinh
		}
	}
	else // neu day la luc minh bi bien thanh ma
	{
		for (auto Temp: GetWorld()->GetGameState()->PlayerArray)
		{
			AMyCharacter* Ghost = Cast<AMyCharacter>(Temp->GetPawn());
			Ghost->SetActorHiddenInGame(false); // tat ca ma khac hien hinh
		}
	}
	
	GetMesh()->SetMaterial(0, DeadMat);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ghost"));
	FTransform Temp = GetActorTransform();
	Temp.SetLocation(DeadLoc);
	if (!HasSpawnDeadBody)
	{
		HasSpawnDeadBody = true;
		GetWorld()->SpawnActor<AActor>(DeadBody, Temp);
	}
}

void AMyCharacter::OnRep_KillByVote()
{
	if (!IsValid(GetController())) // other player is killed and become Ghost
	{
		APlayerController* LocalController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0);
		const bool IsLocalControllerValid = IsValid(LocalController);
		bool IsLocalGhost = false;
		if (IsLocalControllerValid && IsValid(LocalController->GetPawn()))
			IsLocalGhost = Cast<AMyCharacter>(LocalController->GetPawn())->IsGhost;
		if (IsLocalControllerValid && IsLocalGhost)
		{
			SetActorHiddenInGame(false); // visible
		}
		else
		{
			SetActorHiddenInGame(true); // invisible
		}
	}
	else // neu day la luc minh bi bien thanh ma
	{
		for (auto Temp: GetWorld()->GetGameState()->PlayerArray)
		{
			AMyCharacter* Ghost = Cast<AMyCharacter>(Temp->GetPawn());
			Ghost->SetActorHiddenInGame(false); // all ghost are visible
		}
	}
	GetMesh()->SetMaterial(0, DeadMat);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ghost"));
}

void AMyCharacter::ServerOnDead_Implementation(const FVector Loc)
{
	IsGhost = true;
	DeadLoc = Loc;
	GetMesh()->SetMaterial(0, DeadMat);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ghost"));
}

void AMyCharacter::OnHealthChange(const FOnAttributeChangeData& Data)
{
	FVector LocSend = GetActorLocation();
	LocSend.Z = 550.f;
	ServerOnDead(LocSend);
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyCharacter, DeadLoc);
	DOREPLIFETIME(AMyCharacter, IsGhost);
	DOREPLIFETIME(AMyCharacter, IsGhostByVote);
}