// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Actions/SActionEffectBase.h"

#include "Course.h"
#include "GameFramework/GameStateBase.h"
#include "GAS/SActionComponent.h"

void USActionEffectBase::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}

USActionEffectBase::USActionEffectBase()
{
	bAutoStart = true;
}

void USActionEffectBase::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &USActionEffectBase::StopAction, Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &USActionEffectBase::ExecutePeriodicEffect, Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USActionEffectBase::StopAction_Implementation(AActor* Instigator)
{
	/* It could happen that the duration timer calls this before the periodic timer, stopping the last periodic call
	 * avoid this calling it manually before stopping the action */
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	//Clear all the timers
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	// Remove this action from our action component
	USActionComponent* Component = GetOwningComponent();
	RETURN_IF_NULL(Component);
	Component->RemoveAction(this);
	
}

float USActionEffectBase::GetTimeRemaining() const
{
	AGameStateBase* GameState = GetWorld()->GetGameState();
	RETURN_VALUE_IF_NULL(GameState, Duration);
	
	float EndTime = TimeStarted + Duration;
	
	return EndTime - GameState->GetServerWorldTimeSeconds();
}
