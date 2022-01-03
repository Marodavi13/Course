// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/SProjectileMagic.h"

#include "Character/Component/SAttributeComponent.h"
#include "Components/SphereComponent.h"

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

	if(!OtherActor || OtherActor == GetInstigator())
	{
		return;
	}

	USAttributeComponent* AttributeComponent = OtherActor->FindComponentByClass<USAttributeComponent>();

	if(AttributeComponent)
	{
		AttributeComponent->ApplyHealthChange(Damage);

		Destroy();
	}

}
