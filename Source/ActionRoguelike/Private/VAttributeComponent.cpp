// Fill out your copyright notice in the Description page of Project Settings.


#include "VAttributeComponent.h"
#include "VGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);


// Sets default values for this component's properties
UVAttributeComponent::UVAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	RageMax = 0;
	RageMax = 100;

	SetIsReplicatedByDefault(true);
}

bool UVAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool UVAttributeComponent::IsActorAlive(AActor* Actor)
{
	UVAttributeComponent* AttributeComp = GetAttribues(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
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

	/*if (!GetOwner()->HasAuthority())
	{
		return false;
	}*/

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelta = NewHealth - OldHealth;

	// Is Server?
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);

		}
	}

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

float UVAttributeComponent::GetRage() const
{
	return Rage;
}

bool UVAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
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

void UVAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}


void UVAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVAttributeComponent, Health);
	DOREPLIFETIME(UVAttributeComponent, HealthMax);

	//DOREPLIFETIME_CONDITION(UVAttributeComponent, HealthMax, COND_InitialOnly);
}

