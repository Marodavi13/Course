// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

}

bool USAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	Health += DeltaHealth;

	OnHealthChanged.Broadcast(nullptr, this, Health, DeltaHealth);
	
	return true;
}

