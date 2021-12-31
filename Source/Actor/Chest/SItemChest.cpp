// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Chest/SItemChest.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	SetRootComponent(BaseMesh);
	
	UpperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Upper Mesh"));
	UpperMesh->SetupAttachment(BaseMesh);
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	UpperMesh->SetRelativeRotation(FRotator(TargetPitch,0.f,0.f));
}

