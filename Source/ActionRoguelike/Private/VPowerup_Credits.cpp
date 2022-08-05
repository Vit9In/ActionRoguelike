// Fill out your copyright notice in the Description page of Project Settings.


#include "VPowerup_Credits.h"
#include "VPlayerState.h"

AVPowerup_Credits::AVPowerup_Credits()
{
	CreditsAmount = 100;
}

void AVPowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (AVPlayerState* PS = InstigatorPawn->GetPlayerState<AVPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
}	