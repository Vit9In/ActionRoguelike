// Fill out your copyright notice in the Description page of Project Settings.


#include "VActionComponent.h"
#include "VAction.h"

UVActionComponent::UVActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UVActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TSubclassOf<UVAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UVActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

}

void UVActionComponent::AddAction(TSubclassOf<UVAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UVAction* NewAction = NewObject<UVAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UVActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UVAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run; %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UVActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UVAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}
