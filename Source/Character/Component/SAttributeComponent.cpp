// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(!FromActor)
	{
		return nullptr;
	}

	return FromActor->FindComponentByClass<USAttributeComponent>();
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Actor);
	if(!AttributeComponent)
	{
		return false;
	}

	return AttributeComponent->IsAlive();
}

bool USAttributeComponent::ApplyHealthChange(float DeltaHealth, AActor* InstigatorActor)
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
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, DeltaHealth);
	
	return DeltaHealth != 0.f;
}

