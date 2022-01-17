// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

}

bool USAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	/**  Dont heal if we are Max health */
	if(IsFullHealth() && DeltaHealth >= 0.f)
	{
		return false;
	}

	const float PreviousHealth = Health;
	
	/** Set the health and clamp it*/
	Health += DeltaHealth;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	DeltaHealth = Health - PreviousHealth;
	/** Broadcast the event */
	OnHealthChanged.Broadcast(nullptr, this, Health, DeltaHealth);
	
	return DeltaHealth != 0.f;
}

