// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile/SProjectile.h"
#include "SProjectileBlackHole.generated.h"

/**
 * 
 */
UCLASS()
class COURSE_API ASProjectileBlackHole : public ASProjectile
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;
protected:

	/** Scale of the shrinking, in scale units/s*/
	UPROPERTY(EditAnywhere, Category="Black Hole")
	float ShrinkSpeed = 1.f;

	/** The actors that are being shrunk */
	UPROPERTY(Transient, SkipSerialization)
	TSet<AActor*> ShrinkingActors;
	
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void PostInitializeComponents() override;

private:
	UFUNCTION()
	void OnBlackHoleEffectFinish(UParticleSystemComponent* ComponentFinished);

	bool bIsWaitingToBeDestroyed = false;
};
