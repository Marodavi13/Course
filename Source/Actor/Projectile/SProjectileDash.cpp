// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/SProjectileDash.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASProjectileDash::ASProjectileDash()
{
}

void ASProjectileDash::PerformDash(FVector Location, FVector Normal)
{
	if(ensure(GetInstigator()))
	{
		if(Location == FVector::ZeroVector)
		{
			GetInstigator()->TeleportTo(GetActorLocation(), GetInstigator()->GetActorRotation());
		}
		else
		{
			GetInstigator()->TeleportTo(Location - Normal * 25.f, GetInstigator()->GetActorRotation());
		}
	}
	Destroy();
}

void ASProjectileDash::BeginPlay()
{
	Super::BeginPlay();

	const FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ASProjectileDash::PerformDash, FVector::ZeroVector,
									FVector::ZeroVector);
	GetWorldTimerManager().SetTimer(DashHandle, Delegate, DashTime, false);
}

void ASProjectileDash::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnSphereHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 5.f, 8, FColor::Orange, false, 2.f);

	GetWorldTimerManager().ClearTimer(DashHandle);

	//Stop and disable collision
	MovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);

	// Perform the teleport
	PerformDash(Hit.ImpactPoint, Hit.ImpactNormal);
}
