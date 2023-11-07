// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Interface/SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickUpActor.generated.h"

UCLASS()
class COURSE_API ASPickUpActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category= "Activation")
	float ActivationTime = 10.f;

	/** Shows or hides the actor enabling or disabling its collision*/
	void SetPickUpState(bool bIsActive);

	/** Called when the pick up is going to be visible and interactable*/
	virtual void ShowPickUp();

	/** Called when the pick up is going to be hidden and not interactable*/
	UFUNCTION(BlueprintCallable, Category="PickUp")
	virtual void HideAndCooldownPickUp();

};
