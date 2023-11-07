// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COURSE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

	void UpdateInteractWidget();

	void FindBestInteractable();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Interact();

	UFUNCTION(Server, Reliable)
	void Server_Interact(AActor* InteractedActor);
	
protected:

	UPROPERTY(EditAnywhere, Category="Interaction")
	float InteractionDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category="Interaction")
	float InteractionRadius = 25.f;
	
	UPROPERTY(EditAnywhere, Category="Interaction")
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldDynamic;

	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	TSubclassOf<USWorldUserWidget> InteractWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	USWorldUserWidget* InteractWidget;
	
	UPROPERTY(Transient, SkipSerialization)
	AActor* FocusedActor;

	// Called when the game starts
	virtual void BeginPlay() override;
};
