// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;

UCLASS(Blueprintable, BlueprintType)
class COURSE_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectile();
	
protected:
	/** Collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USphereComponent* SphereComponent;

	/** Will give velocity to the actor*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UProjectileMovementComponent* MovementComponent;
	
	/** Brilli brilli*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UParticleSystemComponent* EffectComponent;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UFUNCTION()
	virtual void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) {}
	


};
