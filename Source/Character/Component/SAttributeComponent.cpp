// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

}

bool USAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	/**  Dont heal if we are Max health */
	if(Health == MaxHealth && DeltaHealth >= 0.f)
	{
		return false;
	}
	
	/** Set the health and clamp it*/
	Health += DeltaHealth;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	/** Broadcast the event */
	OnHealthChanged.Broadcast(nullptr, this, Health, DeltaHealth);
	
	return true;
}

