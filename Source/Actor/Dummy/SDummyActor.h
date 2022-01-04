// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/SAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "SDummyActor.generated.h"

class USAttributeComponent;
UCLASS()
class COURSE_API ASDummyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDummyActor();

protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USAttributeComponent* AttributeComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* Mesh;
public:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float DeltaHealth);

};
