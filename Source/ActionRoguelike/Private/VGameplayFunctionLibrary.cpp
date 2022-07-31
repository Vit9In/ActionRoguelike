// Fill out your copyright notice in the Description page of Project Settings.


#include "VGameplayFunctionLibrary.h"
#include "VAttributeComponent.h"

bool UVGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UVAttributeComponent* AttributeComp = UVAttributeComponent::GetAttribues(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, DamageAmount);
	}
	return false;
}

bool UVGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 30000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}

		return true;
	}
	return false;
}
