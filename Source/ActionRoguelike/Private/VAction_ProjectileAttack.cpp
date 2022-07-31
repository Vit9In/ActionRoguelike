// Fill out your copyright notice in the Description page of Project Settings.


#include "VAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UVAction_ProjectileAttack::UVAction_ProjectileAttack()
{
	SpawnLocationName = "HandLocation";
	AttackAnimDelay = 0.2f;
}

void UVAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		// For charge animation
		//Character->PlayAnimMontage(AttackAnim);

		// For charge effect
		//UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), SpawnLocationName /*SpawnLocationName*/, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}

void UVAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensure(ProjectileClass))
	{
		// For set location
		// FVector Location = InstigatorCharacter->GetMesh()->GetSocketLocation("SocketName");
		FVector SpawnLocation = InstigatorCharacter->GetActorLocation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignor Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector InstigatorRotatorVector = InstigatorCharacter->GetControlRotation().Vector();
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (InstigatorRotatorVector * 100); // @fixme: While spaming trace hit to projectile, set close range
		FVector TraceEnd = TraceStart + (InstigatorRotatorVector * 5000);

		FHitResult Hit;
		// Return true if we got to a blocking hit
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			// Overwrite trace end with impact point in world
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - SpawnLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, SpawnLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
