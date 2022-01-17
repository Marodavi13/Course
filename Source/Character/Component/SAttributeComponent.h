// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, Instigator,
	class USAttributeComponent*, OwningComponent, float, NewHealth, float, DeltaHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COURSE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Attributes | Health")
	FOnHealthChanged OnHealthChanged;
	
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintPure, Category = "Attributes | Health")
	bool IsFullHealth() const
	{
		return MaxHealth == Health;
	}

	UFUNCTION(BlueprintCallable, Category = "Attributes | Health")
	bool ApplyHealthChange(float DeltaHealth);

	UFUNCTION(BlueprintPure, Category = "Attributes | Health")
	bool IsAlive() const
	{
		return Health > 0.f;
	}

	UFUNCTION(BlueprintPure, Category = "Attributes | Health")
	float GetCurrentHealth() const
	{
		return Health;
	}

	UFUNCTION(BlueprintPure, Category = "Attributes | Health")
	float GetMaxHealth() const
	{
		return MaxHealth;
	}
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes | Health")
	float Health = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attributes | Health")
	float MaxHealth = Health;
};
