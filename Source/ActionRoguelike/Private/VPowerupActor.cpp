// Fill out your copyright notice in the Description page of Project Settings.


#include "VPowerupActor.h"
#include "Components/SphereComponent.h"

// Sets default values
AVPowerupActor::AVPowerupActor()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
    SphereComp->SetCollisionProfileName("Powerup");
    RootComponent = SphereComp;

    RespawnTime = 10.0f;
}


void AVPowerupActor::ShowPowerup()
{
    SetPowerupState(true);
}

void AVPowerupActor::HideAndCooldownPowerup()
{
    SetPowerupState(false);

    GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AVPowerupActor::ShowPowerup, RespawnTime);
}

void AVPowerupActor::SetPowerupState(bool bNewIsActive)
{
    SetActorEnableCollision(bNewIsActive);

    //Set visibility on root and all children
    RootComponent->SetVisibility(bNewIsActive, true);
}

void AVPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
    // logic in derived classes...
}
