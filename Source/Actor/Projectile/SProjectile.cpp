// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
ASProjectile::ASProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetCollisionProfileName(TEXT("Projectile"));
	SetRootComponent(SphereComponent);
	
	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effects Component"));
	EffectComponent->SetupAttachment(RootComponent);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	MovementComponent->InitialSpeed = 1000.f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

	FlightAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Flight Audio"));
	FlightAudioComponent->SetupAttachment(RootComponent);

}

void ASProjectile::BeginPlay()
{
	Super::BeginPlay();
	FlightAudioComponent->Play();
}

void ASProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASProjectile::OnSphereBeginOverlap);
	SphereComponent->OnComponentHit.AddDynamic(this, &ASProjectile::OnSphereHit);

}

void ASProjectile::PlayExplodeEffects() const
{
	if(!ensure(!IsPendingKill()))
	{
		return;
	}
	
	if(ImpactVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	}

	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(),GetActorRotation());
	}

	if(ImpactCameraShake)
	{
		UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, GetActorLocation(), CameraShakeRadius.X ,CameraShakeRadius.Y);
	}
}

void ASProjectile::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               FVector NormalImpulse, const FHitResult& Hit)
{
	PlayExplodeEffects();
}
