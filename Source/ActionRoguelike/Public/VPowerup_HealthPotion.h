// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VPowerupActor.h"
#include "VPowerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AVPowerup_HealthPotion : public AVPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "HealthPotion")
	int32 CreditCost;
	
public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	AVPowerup_HealthPotion();

};
