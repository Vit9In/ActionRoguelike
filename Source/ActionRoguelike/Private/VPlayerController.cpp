// Fill out your copyright notice in the Description page of Project Settings.


#include "VPlayerController.h"

void AVPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}
