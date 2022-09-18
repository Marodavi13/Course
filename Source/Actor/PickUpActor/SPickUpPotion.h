// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUpActor.h"
#include "SPickUpPotion.generated.h"

UCLASS()
class COURSE_API ASPickUpPotion : public ASPickUpActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickUpPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	
	UPROPERTY(EditAnywhere, Category= "Heal")
	float HealAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Activation")
	int32 CreditsToInteract = 5;
};
