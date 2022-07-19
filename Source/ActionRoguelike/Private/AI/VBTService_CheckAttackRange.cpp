// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UVBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between ai pawn and target actor
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
		if (TargetActor) 
		{
			AAIController* OwnerController = OwnerComp.GetAIOwner();
			if (ensure(OwnerController))
			{
				APawn* AIPawn = OwnerController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(),AIPawn->GetActorLocation());

					bool bWithinRange = DistanceTo < 1500.0f;

					bool bHasLOS = false;
					if (bWithinRange) 
					{
						bHasLOS = OwnerController->LineOfSightTo(TargetActor);
					}
					

					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName,(bWithinRange && bHasLOS));
				}
			}
		}
	}
}