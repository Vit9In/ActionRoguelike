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
class UVActionComponent;

UCLASS()
class ACTIONROGUELIKE_API AVCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVCharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UVInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UVAttributeComponent* AttributeCopm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UVActionComponent* ActionCopm;

	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);	

	void SprintStart();

	void SprintStop();

	void PrimaryInteract();

	void PrimaryAttack();

	void Dash();

	void BlackholeAttack();


	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UVAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;


public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

};
