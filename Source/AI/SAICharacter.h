// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAS/SActionComponent.h"
#include "SAICharacter.generated.h"

class USWorldUserWidget;
class USAttributeComponent;
class UPawnSensingComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTargetChanged, AActor*, NewTarget, AActor*, OldTarget);

UCLASS()
class COURSE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

	UPROPERTY(BlueprintAssignable)
	FOnTargetChanged OnTargetActorChanged;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:

	UPROPERTY(VisibleAnywhere, Category="Sense")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, Category="Attributes")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category="Attributes")
	USActionComponent* ActionComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeOfHitParamName = TEXT("TimeOfHit");

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<USWorldUserWidget> TargetSpottedClass;
	
	UPROPERTY(Transient)
	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(Transient)
	USWorldUserWidget* ActiveTargetSpotted;

	UPROPERTY(Transient, BlueprintReadOnly, Replicated, ReplicatedUsing="OnRep_TargetActor")
	AActor* TargetActor;

	UFUNCTION()
	void OnRep_TargetActor(AActor* OldTarget);
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION()
	void OnPawnSeen(APawn* PawnSeen);
};
