// Fill out your copyright notice in the Description page of Project Settings.


#include "VInteractionComponent.h"
#include "VGameplayInterface.h"
#include <DrawDebugHelpers.h>
#include "VWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

UVInteractionComponent::UVInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UVInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotator;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotator);

	FVector End = EyeLocation + (EyeRotator.Vector() * 500);

	TArray<FHitResult> Hits;

	float Radius = 30.f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	// clear ref before finde
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UVGameplayInterface>())
			{		
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultsWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultsWidgetInstance = CreateWidget<UVWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultsWidgetInstance)
		{
			DefaultsWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultsWidgetInstance->IsInViewport())
			{
				DefaultsWidgetInstance->AddToViewport();
			}			
		}
	}
	else
	{
		if (DefaultsWidgetInstance)
		{
			DefaultsWidgetInstance->RemoveFromParent();
		}
	}

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}
}

void UVInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UVInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	IVGameplayInterface::Execute_Interact(InFocus, MyPawn);
}

