// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PickUpActor/SPickUpPotion.h"

#include "Course.h"
#include "Character/SPlayerState.h"
#include "Character/Component/SAttributeComponent.h"

// Sets default values
ASPickUpPotion::ASPickUpPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASPickUpPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	// We can't use a potion with no attribute comp
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);
	RETURN_IF_NULL(AttributeComponent);
	RETURN_IF_TRUE(AttributeComponent->IsFullHealth());

	// If it is a player, check if it has enough credits
	bool bCanInteractPotion = true;
	ASPlayerState* PlayerState = Cast<ASPlayerState>(InstigatorPawn->Controller->PlayerState);
	if (PlayerState)
	{
		bCanInteractPotion = PlayerState->DoCreditTransaction(-CreditsToInteract, true, TEXT("Purchase Potion"));
	}
	
	RETURN_IF_FALSE(bCanInteractPotion);

	// Apply health change if possible
	if(AttributeComponent->ApplyHealthChange(HealAmount, InstigatorPawn))
	{
		HideAndCooldownPickUp();
	}
}
