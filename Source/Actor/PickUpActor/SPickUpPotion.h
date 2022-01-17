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

protected:
	UPROPERTY(EditAnywhere, Category= "Activation")
	float ActivationTime = 10.f;

	UPROPERTY(EditAnywhere, Category= "Heal")
	float HealAmount;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Shows or hides the actor enabling or disabling its collision*/
	void SetPickUpState(bool bIsActive);

	/** Called when the pick up is going to be visible and interactable*/
	virtual void ShowPickUp();

	/** Called when the pick up is going to be hidden and not interactable*/
	virtual void HideAndCooldownPickUp();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
