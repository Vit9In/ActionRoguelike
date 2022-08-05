// Fill out your copyright notice in the Description page of Project Settings.


#include "VMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "VAttributeComponent.h"
#include "VGameplayFunctionLibrary.h"
#include "VActionComponent.h"
#include "VActionEffect.h"


// Sets default values
AVMagicProjectile::AVMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageAmount = -20.f;

}


void AVMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// More consistent to bind here compared to Constructor which may fail to bind if Blueprint was created before adding this binding (or when using hotreload)
	// PostInitializeComponent is the preferred way of binding any events.
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AVMagicProjectile::OnActorOverlap);
}

void AVMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UVAttributeComponent* AttributeCopm = Cast<UVAttributeComponent>(OtherActor->GetComponentByClass(UVAttributeComponent::StaticClass()));

		//static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Status.Parrying");

		UVActionComponent* ActionComp = Cast<UVActionComponent>(OtherActor->GetComponentByClass(UVActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if (AttributeCopm)
		{
			AttributeCopm->ApplyHealthChange(GetInstigator(), -20.0f);

			Destroy();

			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}

		/*if (UVGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Destroy();   // For bones
		}*/
	}
}



