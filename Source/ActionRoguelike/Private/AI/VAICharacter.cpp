// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VAICharacter.h"

// Sets default values
AVAICharacter::AVAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

