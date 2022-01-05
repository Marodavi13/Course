// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);

	if(Player)
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("MoveToLocation"), Player->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Player);
	}
}
