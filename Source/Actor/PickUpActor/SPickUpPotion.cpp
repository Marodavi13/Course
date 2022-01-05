// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PickUpActor/SPickUpPotion.h"

#include "Character/Component/SAttributeComponent.h"

// Sets default values
ASPickUpPotion::ASPickUpPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASPickUpPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
	if( USAttributeComponent* AttributeComponent = InstigatorPawn->FindComponentByClass<USAttributeComponent>())
	{
		if(AttributeComponent->ApplyHealthChange(HealAmount))
		{
			ActivatePickup();
		}
	}
}

void ASPickUpPotion::ActivatePickup()
{
	/** Hide the pick up and disable collision*/
	ActivatePickUp(false);
	FTimerHandle DummyHandle;

	/** Wait until the activation time has elapsed */
	GetWorldTimerManager().SetTimer(DummyHandle, FTimerDelegate::CreateUObject(this, &ASPickUpPotion::ActivatePickUp, true), ActivationTime, false);
}

// Called when the game starts or when spawned
void ASPickUpPotion::BeginPlay()
{
	Super::BeginPlay();
	ActivatePickup();
}

void ASPickUpPotion::ActivatePickUp(bool bIsVisible)
{
	SetActorHiddenInGame(!bIsVisible);
	SetActorEnableCollision(bIsVisible);
}

// Called every frame
void ASPickUpPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

