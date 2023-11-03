// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Actions/SActionBase.h"
#include "SActionEffectBase.generated.h"

/**
 * 
 */
UCLASS()
class COURSE_API USActionEffectBase : public USActionBase
{
	GENERATED_BODY()

public:
	
	USActionEffectBase();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float Duration = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float Period = 0.5f;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;
	
	UFUNCTION(BlueprintNativeEvent, Category="Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
};
