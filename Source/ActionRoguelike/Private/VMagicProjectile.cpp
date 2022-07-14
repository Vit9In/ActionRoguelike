// Fill out your copyright notice in the Description page of Project Settings.


#include "VMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "VAttributeComponent.h"


// Sets default values
AVMagicProjectile::AVMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AVMagicProjectile::OnActorOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

}

void AVMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UVAttributeComponent* AttributeCopm = Cast<UVAttributeComponent>(OtherActor->GetComponentByClass(UVAttributeComponent::StaticClass()));

		if (AttributeCopm)
		{
			AttributeCopm->ApplyHealthChange(-20.0f);

			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void AVMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

