// Fill out your copyright notice in the Description page of Project Settings.


#include "VAction.h"
#include "VActionComponent.h"


void UVAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	UVActionComponent* Comp = GetOwningCopmonent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UVAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	UVActionComponent* Comp = GetOwningCopmonent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
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

bool UVAction::IsRunning() const
{
	return bIsRunning;
}

UWorld* UVAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}

UVActionComponent* UVAction::GetOwningCopmonent() const
{
	return Cast<UVActionComponent>(GetOuter());
}
