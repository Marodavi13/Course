// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_Heal.generated.h"

/**
 * 
 */
UCLASS()
class COURSE_API USBTTask_Heal : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector TargetToHeal;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
