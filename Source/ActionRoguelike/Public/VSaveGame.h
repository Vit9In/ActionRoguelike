// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UVSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	int32 Credits;
};
