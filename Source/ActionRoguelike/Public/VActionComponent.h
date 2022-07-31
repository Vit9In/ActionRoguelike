// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "VActionComponent.generated.h"

class UVAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UVActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UVActionComponent();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(TSubclassOf<UVAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags");
	FGameplayTagContainer ActiveGameplayTags;

protected:

	UPROPERTY()
	TArray<UVAction*> Actions;

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UVAction>> DefaultActions;

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
