// Fill out your copyright notice in the Description page of Project Settings.


#include "VAction.h"
#include "VActionComponent.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"



void UVAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UVActionComponent* Comp = GetOwningCopmonent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void UVAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	UVActionComponent* Comp = GetOwningCopmonent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

bool UVAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UVActionComponent* Comp = GetOwningCopmonent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

UWorld* UVAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

UVActionComponent* UVAction::GetOwningCopmonent() const
{

	return ActionComp;
	//return Cast<UVActionComponent>(GetOuter());
}

void UVAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UVAction::Initialize(UVActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UVAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void UVAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVAction, RepData);
	DOREPLIFETIME(UVAction, ActionComp);

}