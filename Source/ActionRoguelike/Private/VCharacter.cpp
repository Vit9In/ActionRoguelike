// Fill out your copyright notice in the Description page of Project Settings.


#include "VCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "VInteractionComponent.h"
#include "VAttributeComponent.h"
#include "VActionComponent.h"

// Sets default values
AVCharacter::AVCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);


	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 1.0f;
	JumpMaxCount = 2;

	InteractionComp = CreateDefaultSubobject<UVInteractionComponent>("InteractionComp");
	AttributeCopm = CreateDefaultSubobject<UVAttributeComponent>("AttributeComp");
	ActionCopm = CreateDefaultSubobject<UVActionComponent>("ActionComp");
}

void AVCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeCopm->OnHealthChanged.AddDynamic(this, &AVCharacter::OnHealthChanged);
}

FVector AVCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

// Called when the game starts or when spawned
void AVCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("JumpAction", IE_Pressed, this, &AVCharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AVCharacter::PrimaryAttack);	
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AVCharacter::Dash);
	PlayerInputComponent->BindAction("BlackholeAttack", IE_Pressed, this, &AVCharacter::BlackholeAttack);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AVCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AVCharacter::SprintStop);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AVCharacter::PrimaryInteract);
}

void AVCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeCopm->ApplyHealthChange(this, Amount);
}

// Called when the VCharacter moves forward
void AVCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);

}

// Called when the VCharacter moves right
void AVCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	FVector RihtVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RihtVector, Value);

}

void AVCharacter::SprintStart()
{
	ActionCopm->StartActionByName(this, "Sprint");
}

void AVCharacter::SprintStop()
{
	ActionCopm->StopActionByName(this, "Sprint");
}

void AVCharacter::PrimaryAttack()
{	
	ActionCopm->StartActionByName(this, "PrimaryAttack");
}

void AVCharacter::Dash()
{
	ActionCopm->StartActionByName(this, "Dash");
}

void AVCharacter::BlackholeAttack()
{
	ActionCopm->StartActionByName(this, "Blackhole");
}


void AVCharacter::OnHealthChanged(AActor* InstigatorActor, UVAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void AVCharacter::PrimaryInteract()
{
	if (InteractionComp) 
	{
		InteractionComp->PrimaryInteract();
	}
}

