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
	TSubclassOf<AActor> AttackProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Dash")
	UAnimMontage* DashProjectileAnim;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Dash")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Dash")
	FTimerHandle TimerHandle_Dash;

	UPROPERTY(EditAnywhere, Category = "BlackholeAttack")
	UAnimMontage* BlackholeAttackProjectileAnim;

	UPROPERTY(EditAnywhere, Category = "BlackholeAttack")
	float BlackholeAttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "BlackholeAttack")
	TSubclassOf<AActor> BlackholeAttackProjectileClass;

	UPROPERTY(EditAnywhere, Category = "BlackholeAttack")
	FTimerHandle TimerHandle_BlackholeAttack;

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

	void PrimaryInteract();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	// Called when attacks
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

	// Called when dashes
	void Dash();
	void Dash_TimeElapsed();

	// Called when attacks with blackhole
	void BlackholeAttack();
	void BlackholeAttack_TimeElapsed();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
