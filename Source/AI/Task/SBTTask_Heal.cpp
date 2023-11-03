// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/SBTTask_Heal.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Component/SAttributeComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/** Return if no Blackboard*/
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!ensure(Blackboard))
	{
		return EBTNodeResult::Failed;
	}
	
	/** Get AI controller, return if nullptr*/
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController))
	{
		return EBTNodeResult::Failed;
	}

	/** Get AI character, return if nullptr*/
	APawn* AIPawn = Cast<APawn>(AIController->GetPawn());
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	AActor* HealTarget = Cast<AActor>(Blackboard->GetValueAsObject(TargetToHeal.SelectedKeyName));
	if (!HealTarget)
	{
		HealTarget = AIPawn;
	}

	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(HealTarget);
	if (!AttributeComponent)
	{
		return EBTNodeResult::Failed;
	}

	AttributeComponent->ApplyHealthChange(AttributeComponent->GetMaxHealth(), AIPawn);

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
