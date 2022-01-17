// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USWorldUserWidget;
class USAttributeComponent;
class UPawnSensingComponent;
UCLASS()
class COURSE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category="Sense")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, Category="Attributes")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeOfHitParamName = TEXT("TimeOfHit");

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(Transient)
	USWorldUserWidget* ActiveHealthBar;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION()
	void OnPawnSeen(APawn* PawnSeen);
};
