// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SActionBase.generated.h"

USTRUCT()
struct FSActionRepData
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsActive;

	UPROPERTY()
	AActor* Instigator;
};
class USActionComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class COURSE_API USActionBase : public UObject
{
	GENERATED_BODY()

public:
		
	UPROPERTY(EditAnywhere, Category="Action")
	FName ActionName;

	UPROPERTY(EditAnywhere, Category="Action")
	bool bAutoStart = false;

	void Initialize(USActionComponent* NewOwningComponent);

	UFUNCTION(BlueprintCallable, Category="Action")
	USActionComponent* GetOwningComponent() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator) const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintPure, Category = "Action")
	bool IsActive() const;
	
	UFUNCTION(BlueprintPure, Category = "Action")
	virtual UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
protected:
	
	UPROPERTY(Transient, Replicated)
	USActionComponent* OwningComponent;

	UPROPERTY(Transient, BlueprintReadOnly, Replicated, Category = "Action")
	float TimeStarted;
	
	UPROPERTY(ReplicatedUsing="OnRep_Data")
	FSActionRepData ReplicatedData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	UTexture2D* Icon;
	
	UFUNCTION()
	void OnRep_Data();
	
	// Tags added to owning actor when activated
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	// Action can only start if owner has none of these tags
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
};
