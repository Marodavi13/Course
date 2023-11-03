// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	/** Return if no Blackboard*/
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!ensure(Blackboard))
	{
		return;
	}

	/** Return if no target or AI controller*/
	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(TEXT("TargetActor")));
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!Target || !AIController)
	{
		return;
	}

	/** Retrun if no AI pawn*/
	APawn* AIPawn = AIController->GetPawn();
	if (!ensure(AIPawn))
	{
		return;
	}

	/** Get the sq distance between target and pawn*/
	const float SquaredDistance = FVector::DistSquared(Target->GetActorLocation(), AIPawn->GetActorLocation());
	bool bIsInRange = SquaredDistance < 2000.f * 2000.f;

	if (bIsInRange)
	{
		bIsInRange = AIController->LineOfSightTo(Target);
	}
	
	/** set the calculated value in the bb*/
	Blackboard->SetValueAsBool(AttackRangeKey.SelectedKeyName, bIsInRange);
}
