// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Dummy/SDummyActor.h"

// Sets default values
ASDummyActor::ASDummyActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attribute Component"));
}

void ASDummyActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASDummyActor::OnHealthChanged);
}

void ASDummyActor::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth,
	float DeltaHealth)
{
	if(DeltaHealth < 0.f)
	{
		Mesh->SetScalarParameterValueOnMaterials(TEXT("TimeToHit"), GetWorld()->TimeSeconds);
	}
}

