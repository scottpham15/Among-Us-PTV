// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "DeadBody.h"
#include "MyCharacter.generated.h"

class UMyAttributeSet;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroyComponent, ADeadBody*, DeadBodyDes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAwayFromDeadBody);

UCLASS(Blueprintable, config=Game)
class MULTIPLAYERTEMPLATE_API AMyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

public:

	virtual void PossessedBy(AController* NewController) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	UMyAttributeSet* BasicAttributeSet;

	UPROPERTY(BlueprintReadWrite, Replicated)
	bool IsGhost;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_KillByVote)
	bool IsGhostByVote;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetupACS();

	void DeadBodyReported(ADeadBody* DeadBodyDes);
	void AwayFromTheDeadBody();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSubclassOf<AActor> DeadBody;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UMaterial* DeadMat;

	UFUNCTION()
	void OnRep_IsDead();

	UFUNCTION()
	void OnRep_KillByVote();

	UFUNCTION(Server, Unreliable)
	void ServerOnDead(FVector Loc);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsDead)
	FVector DeadLoc;

	bool HasSpawnDeadBody = false;
	
	UPROPERTY(BlueprintAssignable)
	FDestroyComponent FoundDeadBody;
	
	UPROPERTY(BlueprintAssignable)
	FAwayFromDeadBody AwayFromDeadBody;
protected:
	void OnHealthChange(const FOnAttributeChangeData& Data);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
