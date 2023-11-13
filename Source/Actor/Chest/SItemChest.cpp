// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Chest/SItemChest.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	SetRootComponent(BaseMesh);
	
	UpperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Upper Mesh"));
	UpperMesh->SetupAttachment(BaseMesh);

	bReplicates = true;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bIsLidOpen = !bIsLidOpen;
	OnRep_IsLidOpen();
}

void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_IsLidOpen();
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bIsLidOpen);
}

void ASItemChest::OnRep_IsLidOpen()
{
	float WantedPitch = bIsLidOpen ? TargetPitch : 0.f;
	UpperMesh->SetRelativeRotation(FRotator(WantedPitch,0.f,0.f));
}

