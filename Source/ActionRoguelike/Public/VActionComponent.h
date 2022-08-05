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
	void AddAction(AActor* Instigator, TSubclassOf<UVAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UVAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UVAction* GetAction(TSubclassOf<UVAction> ActionClass) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags");
	FGameplayTagContainer ActiveGameplayTags;

protected:

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UVAction>> DefaultActions;

	UPROPERTY(Replicated)
	TArray<UVAction*> Actions;	

	virtual void BeginPlay() override;

public:	

	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
