// Fill out your copyright notice in the Description page of Project Settings.


#include "VAttributeComponent.h"

// Sets default values for this component's properties
UVAttributeComponent::UVAttributeComponent()
{
	Health = 100;

}

bool UVAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UVAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

