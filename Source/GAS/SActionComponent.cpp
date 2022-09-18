// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SActionComponent.h"

#include "Course.h"

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USActionComponent::AddAction(TSubclassOf<USActionBase> AddedActionClass)
{
	ensure(AddedActionClass);
	RETURN_IF_NULL(AddedActionClass);

	// Instanciate the action
	USActionBase* AddedAction = NewObject<USActionBase>(this, AddedActionClass);
	RETURN_IF_NULL(AddedAction);

	
	Actions.Add(AddedAction);

}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(USActionBase* Action: Actions)
	{
		// I action not valid or not matching name, continue
		if(!Action || Action->ActionName != ActionName)
		{
			continue;
		}
		
		Action->StartAction(Instigator);
		return true;
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for(USActionBase* Action: Actions)
	{
		// I action not valid or not matching name, continue
		if (!Action || Action->ActionName != ActionName)
		{
			continue;
		}
		
		Action->StopAction(Instigator);
		return true;
	}
	return false;
}

