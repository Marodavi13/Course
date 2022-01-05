// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Interface/SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickUpPotion.generated.h"

UCLASS()
class COURSE_API ASPickUpPotion : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickUpPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	void ActivatePickup();
protected:
	UPROPERTY(EditAnywhere, Category= "Activation")
	float ActivationTime = 10.f;

	UPROPERTY(EditAnywhere, Category= "Heal")
	float HealAmount;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ActivatePickUp(bool bIsVisible = true);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
