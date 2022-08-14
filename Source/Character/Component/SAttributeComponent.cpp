// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/SAttributeComponent.h"

#include "Course.h"
#include "SGameModeBase.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("su.DamageMultiplier"),
	1.f,
	TEXT("Sets a damage multiplier applied to every damage"),
	ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (!FromActor)
	{
		return nullptr;
	}

	return FromActor->FindComponentByClass<USAttributeComponent>();
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Actor);
	if (!AttributeComponent)
	{
		return false;
	}

	return AttributeComponent->IsAlive();
}

bool USAttributeComponent::ApplyHealthChange(float DeltaHealth, AActor* InstigatorActor)
{
	RETURN_VALUE_IF_TRUE(GetOwner()->CanBeDamaged() && DeltaHealth < 0.f, false);

	/**  Dont heal if we are Max health */
	RETURN_VALUE_IF_TRUE(IsFullHealth() && DeltaHealth >= 0.f, false);

	if (DeltaHealth < 0.f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		DeltaHealth *= DamageMultiplier;
	}
	
	const float PreviousHealth = Health;
	
	/** Set the health and clamp it*/
	Health += DeltaHealth;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	DeltaHealth = Health - PreviousHealth;
	/** Broadcast the event */
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, DeltaHealth);

	// Has died?
	if (DeltaHealth < 0.f && Health == 0.f)
	{
		ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		RETURN_VALUE_IF_NULL(GameMode, DeltaHealth != 0.f);
		GameMode->OnActorKilled(GetOwner(), InstigatorActor);
	}
	return DeltaHealth != 0.f;
}

bool USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(-GetMaxHealth(),Instigator);
}

