// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VPowerupActor.h"
#include "VPowerup_Action.generated.h"

class UVAction;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AVPowerup_Action : public AVPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<UVAction> ActionToGrant;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;
};
