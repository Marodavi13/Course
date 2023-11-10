// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, Instigator,
	class USAttributeComponent*, OwningComponent, float, NewValue, float, DeltaValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COURSE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable, Category="Attributes | Health")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Attributes | Rage")
	FOnAttributeChanged OnRageChanged;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintPure, Category = "Attributes")
	static bool IsActorAlive(AActor* Actor);
	
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintPure, Category = "Attributes | Health")
	bool IsFullHealth() const
	{
		return MaxHealth == Health;
	}

	UFUNCTION(BlueprintCallable, Category = "Attributes | Health")
	bool ApplyHealthChange(float DeltaHealth, AActor* InstigatorActor);

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

	bool Kill(AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes | Rage")
	bool ApplyRageChange(float DeltaRage, AActor* InstigatorActor);

	UFUNCTION(BlueprintPure, Category = "Attributes | Rage")
	float GetCurrentRage() const
	{
		return Rage;
	}

	UFUNCTION(BlueprintPure, Category = "Attributes | Rage")
	float GetMaxRage() const
	{
		return MaxRage;
	}

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes | Health")
	float Health = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category="Attributes | Health")
	float MaxHealth = Health;

	UPROPERTY(Transient, BlueprintReadOnly, Replicated, Category="Attributes | Rage")
	float Rage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes | Rage", meta=(ClampMin = 0))
	float RagePerHealthLost = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes | Rage")
	float MaxRage = 100.f;

	//Reliable because it handles dying logic
	UFUNCTION(NetMulticast, Reliable) //TODO make unrealble once we move the die logic from SCharacter
	void Multicast_HealthChanged(AActor* Instigator, float NewHealth, float DeltaHealth);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_RageChanged(AActor* Instigator, float NewRage, float DeltaRage);

};
