// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VAction.h"
#include "VAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UVAction_ProjectileAttack : public UVAction
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	FName SpawnLocationName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	UVAction_ProjectileAttack();
};
