#include "GAS/SActionComponent.h"
#include "Course.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_STANFORD);

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}

USActionComponent* USActionComponent::Get(const AActor* Owner)
{
	return Owner ? Owner->FindComponentByClass<USActionComponent>() : nullptr;
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority())
	{
		for (const auto ActionClass : InitialActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void USActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Copy to avoid check about removing element during for loop iteration
	TArray<USActionBase*> CopiedActions = Actions;
	for (USActionBase* Action : CopiedActions)
	{
		if(Action && Action->IsActive())
		{
			Action->StopAction(GetOwner());
		}
	}
	
	Super::EndPlay(EndPlayReason);

	
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	for (USActionBase* Action: Actions)
	{
		FColor Color = Action->IsActive() ? FColor::Green : FColor::Red;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action %s"),	*GetNameSafe(GetOwner()), *GetNameSafe(Action));
		
		LogOnScreen(this, ActionMsg, Color, 0.0f);
	}
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USActionBase> AddedActionClass)
{
	ensure(AddedActionClass);
	RETURN_IF_NULL(AddedActionClass);

	if(!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to add action [%s]"), *GetNameSafe(AddedActionClass))
		return;
	}
	
	// Instanciate the action
	USActionBase* AddedAction = NewObject<USActionBase>(GetOwner(), AddedActionClass);
	RETURN_IF_NULL(AddedAction);

	AddedAction->Initialize(this);
	Actions.Add(AddedAction);
	
	if (AddedAction->bAutoStart && ensure(AddedAction->CanStart(Instigator)))
	{
		AddedAction->StartAction(Instigator);
	}
	
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);
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

		//Am I a client?
		if(!GetOwner()->HasAuthority())
		{
			Server_StartAction(Instigator, ActionName);
		}

		//UE Insights Bookmark
		TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));
		
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

		//Am I a client?
		if(!GetOwner()->HasAuthority())
		{
			Server_StopAction(Instigator, ActionName);
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

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}

void USActionComponent::Server_StartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::Server_StopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for(USActionBase* Action : Actions)
	{
		if(Action)
		{
			bWroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}
