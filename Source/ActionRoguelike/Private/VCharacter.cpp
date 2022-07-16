// Fill out your copyright notice in the Description page of Project Settings.


#include "VCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "VInteractionComponent.h"
#include "VAttributeComponent.h"

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

	DashAnimDelay = 0.2f;
	AttackAnimDelay = 0.2f;
	BlackholeAttackAnimDelay = 0.2f;
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

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AVCharacter::PrimaryInteract);
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

// Called when the VCharacter attacks
void AVCharacter::PrimaryAttack()
{	
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AVCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);
}

void AVCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(AttackProjectileClass);	

	/*if (ensureAlways(ProjectileClass))
	{

		FTransform SpawnTM = FTransform(GetControlRotation(), GetActorLocation());

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}*/
}

void AVCharacter::Dash()
{
	PlayAnimMontage(DashProjectileAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &AVCharacter::Dash_TimeElapsed, DashAnimDelay);
}

void AVCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void AVCharacter::BlackholeAttack()
{
	PlayAnimMontage(BlackholeAttackProjectileAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &AVCharacter::BlackholeAttack_TimeElapsed, BlackholeAttackAnimDelay);
}

void AVCharacter::BlackholeAttack_TimeElapsed()
{
	SpawnProjectile(BlackholeAttackProjectileClass);
}

void AVCharacter::PrimaryInteract()
{
	if (InteractionComp) 
	{
		InteractionComp->PrimaryInteract();
	}
}

void AVCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensure(ClassToSpawn))
	{
		// For set location
		// FVector Location = GetMesh()->GetSocketLocation("SocketName");
		FVector SpawnLocation = GetActorLocation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignor Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = CameraComp->GetComponentLocation();

		// Endpoint far into the look-at distance
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FHitResult Hit;
		// Return true if we got to a blocking hit
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			// Overwrite trace end with impact point in world
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - SpawnLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, SpawnLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

