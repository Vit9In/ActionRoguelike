// Fill out your copyright notice in the Description page of Project Settings.


#include "VActionComponent.h"
#include "VAction.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

UVActionComponent::UVActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UVActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Server only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UVAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
	
}

void UVActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Draw All Actions
	for (UVAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s"),
			*GetNameSafe(GetOwner()),
			*GetNameSafe(Action),
			Action->IsRunning() ? TEXT("true") : TEXT("false"));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UVActionComponent::AddAction(AActor* Instigator, TSubclassOf<UVAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	// Skip for clients
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	UVAction* NewAction = NewObject<UVAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
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

			// Is Client?
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
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
			// Is Client?
			if (!GetOwner()->HasAuthority())
			{
				ServerStopAction(Instigator, ActionName);
			}

			Action->StopAction(Instigator);
			return true;		
		}
	}

	return false;
}

void UVActionComponent::RemoveAction(UVAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

UVAction* UVActionComponent::GetAction(TSubclassOf<UVAction> ActionClass) const
{
	for (UVAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

void UVActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UVActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool UVActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSometing = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UVAction* Action : Actions)
	{
		if (Action)
		{
			WroteSometing |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSometing;
}

void UVActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVActionComponent, Actions);
}
