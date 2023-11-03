// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SActionBase.generated.h"

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

protected:
	
	bool bIsActive = false;

	// Tags added to owning actor when activated
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	// Action can only start if owner has none of these tags
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category="Action")
	USActionComponent* GetOwningComponent() const; 

};
