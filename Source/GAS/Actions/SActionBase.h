// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SActionBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class COURSE_API USActionBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);
	
	UPROPERTY(EditAnywhere, Category="Action")
	FName ActionName;

	UFUNCTION(BlueprintPure, Category = "Action")
	virtual UWorld* GetWorld() const override;
};
