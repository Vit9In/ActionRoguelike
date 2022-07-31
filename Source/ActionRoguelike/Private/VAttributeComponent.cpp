// Fill out your copyright notice in the Description page of Project Settings.


#include "VAttributeComponent.h"
#include "VGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

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

bool UVAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
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
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	//Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		AVGameModeBase* GM = Cast<AVGameModeBase>(GetWorld()->GetAuthGameMode<AVGameModeBase>());
		if (GM) 
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

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

