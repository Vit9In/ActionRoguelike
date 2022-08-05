// Fill out your copyright notice in the Description page of Project Settings.


#include "VPowerup_Action.h"
#include "VActionComponent.h"
#include "VAction.h"

void AVPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	// Make sure we have instigator & that action class was set up
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	UVActionComponent* ActionComp = Cast<UVActionComponent>(InstigatorPawn->GetComponentByClass(UVActionComponent::StaticClass()));
	// Check if Player already has action class
	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant))
		{
			//UE_LOG(LogTemp, Log, TEXT("Instigator already has action of class: %s"), *GetNameSafe(ActionToGrant));
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}

		// Give new Ability
		ActionComp->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldownPowerup();
	}
}