// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PickUpActor/SPickUpActor.h"
#include "SPickUpCoin.generated.h"

/**
 * 
 */
UCLASS()
class COURSE_API ASPickUpCoin : public ASPickUpActor
{
	GENERATED_BODY()

public:

	ASPickUpCoin();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
protected:
	UPROPERTY(EditAnywhere, Category= "Credits")
	int32 CreditsOnInteract = 1;

};
