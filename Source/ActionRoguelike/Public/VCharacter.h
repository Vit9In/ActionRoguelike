// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UVInteractionComponent;
class UAnimMontage;
class UVAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AVCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVCharacter();

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UVInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UVAttributeComponent* AttributeCopm;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the VCharacter moves forward
	void MoveForward(float Value);

	// Called when the VCharacter moves right
	void MoveRight(float Value);

	// Called when the VCharacter attacks
	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void PrimaryInteract();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
