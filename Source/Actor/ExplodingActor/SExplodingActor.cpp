#include "SExplodingActor.h"

#include "DrawDebugHelpers.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASExplodingActor::ASExplodingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Mesh->SetSimulatePhysics(true);
	// Enabling Simulate physics automatically changes the Profile to PhysicsActor in Blueprint, in C++ we need to change this manually.
	Mesh->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName); 
	SetRootComponent(Mesh);
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force"));
	RadialForceComponent->SetupAttachment(Mesh);
	RadialForceComponent->Radius = 350.f;
	RadialForceComponent->Falloff = RIF_Linear;
	RadialForceComponent->ImpulseStrength = 2000.f;
	RadialForceComponent->bImpulseVelChange = true;
}

void ASExplodingActor::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();
	UE_LOG(LogTemp, Log, TEXT("Mesh has been hit in %s by %s"), *GetNameSafe(this), *GetNameSafe(OtherActor));

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.f, true);
}

void ASExplodingActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Mesh->OnComponentHit.AddUniqueDynamic(this, &ASExplodingActor::OnMeshHit);
}