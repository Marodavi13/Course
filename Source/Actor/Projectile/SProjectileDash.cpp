// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/SProjectileDash.h"

#include "DrawDebugHelpers.h"

ASProjectileDash::ASProjectileDash()
{
}

void ASProjectileDash::PerformDash(FVector Location, FVector Normal)
{
	if(Location == FVector::ZeroVector)
	{
		GetInstigator()->SetActorLocation(GetActorLocation());
	}
	else
	{
		GetInstigator()->SetActorLocation(Location - Normal * 25.f);
	}
	OnDashFinished.Broadcast();
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
	
	PerformDash(Hit.ImpactPoint, Hit.ImpactNormal);
}
