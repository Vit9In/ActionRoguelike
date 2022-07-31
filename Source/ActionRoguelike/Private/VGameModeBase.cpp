// Fill out your copyright notice in the Description page of Project Settings.


#include "VGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/VAICharacter.h"
#include "VAttributeComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include <VCharacter.h>

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

AVGameModeBase::AVGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AVGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether its allowed to spawn determined by spawn logic later in the chain...
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AVGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void AVGameModeBase::KillALl()
{
	for (TActorIterator<AVAICharacter> It(GetWorld()); It; ++It)
	{
		AVAICharacter* Bot = *It;

		UVAttributeComponent* AttributeComp = UVAttributeComponent::GetAttribues(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // pass in player? for kill credit
		}
	}
}

void AVGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CvarSpawnBots'."));
		return;
	}

	int32 NumOfAliveBots = 0;
	for (TActorIterator<AVAICharacter> It(GetWorld()); It; ++It)
	{
		AVAICharacter* Bot = *It;

		UVAttributeComponent* AttributeComp = UVAttributeComponent::GetAttribues(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NumOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NumOfAliveBots);

	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AVGameModeBase::OnQueryCompleted);
	}

}

void AVGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}	

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		// Track all the used spawn locations
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void AVGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void AVGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AVCharacter* Player = Cast<AVCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}
}

