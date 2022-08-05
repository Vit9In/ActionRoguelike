// Fill out your copyright notice in the Description page of Project Settings.


#include "VItemChest.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AVItemChest::AVItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMedh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetUp = 100;

	bReplicates = true;
	//SetReplicates(true);
}

void AVItemChest::Interact_Implementation(APawn* InstigatorPawn)
{	
	bLidOpened = !bLidOpened;	
	OnRep_LidOpened();
}

void AVItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetUp : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

void AVItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVItemChest, bLidOpened);
}

