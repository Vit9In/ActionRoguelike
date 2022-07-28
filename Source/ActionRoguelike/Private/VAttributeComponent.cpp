// Fill out your copyright notice in the Description page of Project Settings.


#include "VAttributeComponent.h"

bool UVAttributeComponent::IsActorAlive(AActor* Actor)
{
	UVAttributeComponent* AttributeComp = GetAttribues(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

// Sets default values for this component's properties
UVAttributeComponent::UVAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}

bool UVAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UVAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UVAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UVAttributeComponent::ApplyHealthChange(AActor* InstigatorActor ,float Delta)
{	
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;
}

UVAttributeComponent* UVAttributeComponent::GetAttribues(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UVAttributeComponent>(FromActor->GetComponentByClass(UVAttributeComponent::StaticClass()));
	}

	return nullptr;
}

