// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VTargetDummy.generated.h"

class UVAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AVTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVTargetDummy();

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Compomemt")
	UVAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Compomemt")
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UVAttributeComponent* OwningComp, float NewHealth, float Delta);

public:	

};
