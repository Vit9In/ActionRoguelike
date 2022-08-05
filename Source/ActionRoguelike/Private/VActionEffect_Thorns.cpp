// Fill out your copyright notice in the Description page of Project Settings.


#include "VActionEffect_Thorns.h"
#include "VAttributeComponent.h"
#include "VActionComponent.h"
#include "VGameplayFunctionLibrary.h"


UVActionEffect_Thorns::UVActionEffect_Thorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	Period = 0.0f;
}

void UVActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UVAttributeComponent* Attributes = UVAttributeComponent::GetAttribues(GetOwningCopmonent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &UVActionEffect_Thorns::OnHealthChanged);
	}
}

void UVActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	UVAttributeComponent* Attributes = UVAttributeComponent::GetAttribues(GetOwningCopmonent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &UVActionEffect_Thorns::OnHealthChanged);
	}
}


void UVActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UVAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningCopmonent()->GetOwner();

	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		ReflectedAmount = FMath::Abs(ReflectedAmount);

		UVGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}

