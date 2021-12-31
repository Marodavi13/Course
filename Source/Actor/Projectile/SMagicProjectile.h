// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SMagicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;

UCLASS(Blueprintable, BlueprintType)
class COURSE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	/** Collision */
	UPROPERTY(VisibleAnywhere, Category="Component")
	USphereComponent* SphereComponent;

	/** Will give velocity to the actor*/
	UPROPERTY(VisibleAnywhere, Category="Component")
	UProjectileMovementComponent* MovementComponent;
	
	/** Brilli brilli*/
	UPROPERTY(VisibleAnywhere, Category="Component")
	UParticleSystemComponent* EffectComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
