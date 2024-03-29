// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VActionEffect.h"
#include "VActionEffect_Thorns.generated.h"

class UVAttributeComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UVActionEffect_Thorns : public UVActionEffect
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UVAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UVActionEffect_Thorns();
};
