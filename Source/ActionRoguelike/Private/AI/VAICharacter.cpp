// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "VAttributeComponent.h"
#include "BrainComponent.h"
#include "VWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AVAICharacter::AVAICharacter()
{
	PawnSensigComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeCopm = CreateDefaultSubobject<UVAttributeComponent>("AttributeComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// For Bones
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	//GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParamName = "TimeToHit";

}

void AVAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensigComp->OnSeePawn.AddDynamic(this, &AVAICharacter::OnPawnSeen);
	AttributeCopm->OnHealthChanged.AddDynamic(this, &AVAICharacter::OnHealthChanged);
}

void AVAICharacter::SetTargetActor(AActor* NewTarget)
{

	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void AVAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void AVAICharacter::OnHealthChanged(AActor* InstigatorActor, UVAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UVWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		UE_LOG(LogTemp, Warning, TEXT("Hit"));

		//Died
		if (NewHealth <= 0.0f)
		{
			// stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			// regdol
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// set lifespan
			SetLifeSpan(10.0f);
		}
	}
}
