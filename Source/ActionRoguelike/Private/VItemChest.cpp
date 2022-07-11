// Fill out your copyright notice in the Description page of Project Settings.


#include "VItemChest.h"

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
}

void AVItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeLocation(FVector(0, TargetUp, 0));

}

// Called when the game starts or when spawned
void AVItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

