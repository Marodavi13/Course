// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraShakeBase.h"

#include "SProjectile.generated.h"

class USoundCue;
class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class UCameraShakeBase;

UCLASS(Abstract)
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UAudioComponent* FlightAudioComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Effects")
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Effects")
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Effects")
	FVector2D CameraShakeRadius = FVector2D(0.f, 150.f);
	
	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;

	void PlayExplodeEffects() const;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UFUNCTION()
	virtual void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
	


};
