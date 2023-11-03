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

USActionComponent* USActionComponent::Get(const AActor* Owner)
{
	return Owner ? Owner->FindComponentByClass<USActionComponent>() : nullptr;
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const auto ActionClass : InitialActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString DebugMessage = GetNameSafe(GetOwner()) + TEXT(": ") + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, DebugMessage);
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USActionBase> AddedActionClass)
{
	ensure(AddedActionClass);
	RETURN_IF_NULL(AddedActionClass);

	// Instanciate the action
	USActionBase* AddedAction = NewObject<USActionBase>(this, AddedActionClass);
	RETURN_IF_NULL(AddedAction);
	
	Actions.Add(AddedAction);
	if (AddedAction->bAutoStart && ensure(AddedAction->CanStart(Instigator)))
	{
		AddedAction->StartAction(Instigator);
	}
	
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(USActionBase* Action: Actions)
	{
		// I action not valid or not matching name, continue
		if (!Action || Action->ActionName != ActionName)
		{

			continue;
		}

		if (!Action->CanStart(Instigator))
		{
			FString DebugMsg = FString::Printf(TEXT("Failed to start action (%s)"), *ActionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, DebugMsg);
			continue;
		}
		
		Action->StartAction(Instigator);
		return true;
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USActionBase* Action: Actions)
	{
		// I action not valid or not matching name, continue
		if (!Action || Action->ActionName != ActionName || !Action->IsActive())
		{
			continue;
		}
		
		Action->StopAction(Instigator);
		return true;
	}
	return false;
}

void USActionComponent::RemoveAction(USActionBase* ActionToRemove)
{
	RETURN_IF_NULL(ActionToRemove);
	RETURN_IF_TRUE(ActionToRemove->IsActive());
	
	Actions.Remove(ActionToRemove);
}

