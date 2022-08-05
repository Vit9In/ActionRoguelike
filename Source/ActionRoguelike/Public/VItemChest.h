// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VGameplayInterface.h"
#include "VItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AVItemChest : public AActor, public IVGameplayInterface
{
	GENERATED_BODY()	
	
public:

	UPROPERTY(EditAnywhere)
	float TargetUp;

protected:

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened",BlueprintReadOnly) //RepNotify
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;

public:	

	// Sets default values for this actor's properties
	AVItemChest();

	void Interact_Implementation(APawn* InstigatorPawn);
};
