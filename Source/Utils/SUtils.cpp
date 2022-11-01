// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/SUtils.h"

#include "Course.h"
#include "Character/Component/SAttributeComponent.h"

bool USUtils::ApplyDamage(AActor* DamageInstigator, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(TargetActor);
	RETURN_VALUE_IF_NULL(AttributeComponent, false);

	DamageAmount = - FMath::Sign(DamageAmount) * DamageAmount;
	return AttributeComponent->ApplyHealthChange(DamageAmount, DamageInstigator);
}

bool USUtils::ApplyDirectionalDamage(AActor* DamageInstigator, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	const bool bDamageApplied = ApplyDamage(DamageInstigator, TargetActor, DamageAmount);
	RETURN_VALUE_IF_FALSE(bDamageApplied, false);
	
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
	const bool bIsSimulatingPhysics = HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName);
	RETURN_VALUE_IF_FALSE(bIsSimulatingPhysics, false);

	// Direction = Target - Origin
	FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
	Direction.Normalize();
	HitComponent->AddImpulseAtLocation(Direction * 100000.f, HitResult.ImpactPoint, HitResult.BoneName);

	return true;
}
