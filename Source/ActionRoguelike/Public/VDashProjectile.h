// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VProjectileBase.h"
#include "VDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AVDashProjectile : public AVProjectileBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;

public:

	AVDashProjectile();
	
};
