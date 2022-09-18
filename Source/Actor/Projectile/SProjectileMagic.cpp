// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/SProjectileMagic.h"

#include "Course.h"
#include "Character/Component/SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Utils/SUtils.h"

void ASProjectileMagic::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASProjectileMagic::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep,
	                            SweepResult);
	RETURN_IF_NULL(OtherActor);
	RETURN_IF_TRUE(OtherActor == GetInstigator());
	RETURN_IF_TRUE(OtherActor->GetInstigator() == GetInstigator());

	USUtils::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult);	
	
	PlayExplodeEffects();
	Destroy();
}
