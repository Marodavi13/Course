// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Component/SAttributeComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	/** Return if no Blackboard*/
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if(!ensure(Blackboard))
	{
		return;
	}

	AAIController* AIController = OwnerComp.GetAIOwner();

	if(!AIController)
	{
		return;
	}

	/** Retrun if no AI pawn*/
	APawn* AIPawn = AIController->GetPawn();
	if(!ensure(AIPawn))
	{
		return;
	}

	/** Return if no attributes*/
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(AIPawn);
	if(!AttributeComponent)
	{
		return;
	}

	/** Set the low health bool in the BB*/
	Blackboard->SetValueAsBool(IsLowHealthKey.SelectedKeyName, AttributeComponent->GetCurrentHealth() < LowHealthThreshold);
}
