// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PickUpActor/SPickUpActor.h"

// Sets default values
ASPickUpActor::ASPickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASPickUpActor::BeginPlay()
{
	Super::BeginPlay();
	HideAndCooldownPickUp();
}

void ASPickUpActor::SetPickUpState(bool bIsActive)
{
	SetActorHiddenInGame(!bIsActive);
	SetActorEnableCollision(bIsActive);
}

void ASPickUpActor::ShowPickUp()
{
	SetPickUpState(true);
}

void ASPickUpActor::HideAndCooldownPickUp()
{
	SetPickUpState(false);
	FTimerHandle DummyHandle;

	/** Wait until the activation time has elapsed */
	GetWorldTimerManager().SetTimer(DummyHandle,this, &ASPickUpActor::ShowPickUp, ActivationTime, false);
}


