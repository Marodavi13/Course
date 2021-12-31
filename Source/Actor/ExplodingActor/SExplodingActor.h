// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplodingActor.generated.h"

class URadialForceComponent;

UCLASS()
class COURSE_API ASExplodingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplodingActor();

protected:
	/** The mesh that will receive the Hit*/
	UPROPERTY(VisibleAnywhere, Category="Component")
	UStaticMeshComponent* Mesh;

	/** The force component that will apply the impulse when hit*/
	UPROPERTY(VisibleAnywhere, Category="Component")
	URadialForceComponent* RadialForceComponent;

	/** The function that will be bound to the OnHit Callback*/
	UFUNCTION()
	virtual void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Add the binding here*/
	virtual void PostInitializeComponents() override;
};
