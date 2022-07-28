// Fill out your copyright notice in the Description page of Project Settings.


#include "VPowerup_HealthPotion.h"
#include "VAttributeComponent.h"

AVPowerup_HealthPotion::AVPowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void AVPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UVAttributeComponent* AttributeCopm = Cast<UVAttributeComponent>(InstigatorPawn->GetComponentByClass(UVAttributeComponent::StaticClass()));

	if (ensure(AttributeCopm) && !AttributeCopm->IsFullHealth())
	{
		if (AttributeCopm->ApplyHealthChange(this, AttributeCopm->GetHealthMax()))
		{
			HideAndCooldownPowerup();
		}
	}
}


