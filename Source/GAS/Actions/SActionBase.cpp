#include "GAS/Actions/SActionBase.h"
#include "Course.h"
#include "GAS/SActionComponent.h"
#include "Net/UnrealNetwork.h"

void USActionBase::StartAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this, FString::Printf(TEXT("Started Action %s by %s"), *GetNameSafe(this), *GetNameSafe(Instigator)), FColor::Green);
	//UE_LOG(LogTemp, Log, TEXT("Started Action %s by %s"), *GetNameSafe(this), *GetNameSafe(Instigator));

	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsActive = true;
}

void USActionBase::StopAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this, FString::Printf(TEXT("Stopped Action %s by %s"), *GetNameSafe(this), *GetNameSafe(Instigator)), FColor::White);
	//UE_LOG(LogTemp, Log, TEXT("Stopped Action %s by %s"), *GetNameSafe(this), *GetNameSafe(Instigator));

	//ensureAlways(IsActive());
	
	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsActive = false;
}

bool USActionBase::CanStart_Implementation(AActor* Instigator) const
{
	USActionComponent* ActionComponent = GetOwningComponent();

	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	if (IsActive())
	{
		return false;
	}

	return true;
}

void USActionBase::Initialize(USActionComponent* NewOwningComponent)
{
	OwningComponent = NewOwningComponent; 
}

bool USActionBase::IsActive() const
{
	return bIsActive;
}

UWorld* USActionBase::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if(Actor)
	{
		return Actor->GetWorld();
	}
	const UActorComponent* OuterComponent = Cast<UActorComponent>(GetOuter());
	if(OuterComponent)
	{
		return OuterComponent->GetWorld();
	}

	return nullptr;
}

void USActionBase::OnRep_IsActive()
{
	if(bIsActive)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

USActionComponent* USActionBase::GetOwningComponent() const
{
	return OwningComponent;
}

void USActionBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionBase, bIsActive);
	DOREPLIFETIME(USActionBase, OwningComponent);
}

