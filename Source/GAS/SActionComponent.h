// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Actions/SActionBase.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USActionBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COURSE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USActionComponent();

	static USActionComponent* Get(const AActor* Owner);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void AddAction(AActor* Instigator, TSubclassOf<USActionBase> AddedActionClass);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	void RemoveAction(USActionBase* ActionToRemove);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Tags")
	FGameplayTagContainer ActiveGameplayTags;
protected:
	
	UPROPERTY(EditAnywhere, Category="Action")
	TArray<TSubclassOf<USActionBase>> InitialActions;
	
	UPROPERTY(Transient, SkipSerialization, BlueprintReadOnly, Category="Action")
	TArray<USActionBase*> Actions;
	
	virtual void BeginPlay() override;	
};
