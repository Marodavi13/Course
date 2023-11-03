#include "GAS/Actions/SActionBase.h"
#include "Course.h"
#include "GAS/SActionComponent.h"

void USActionBase::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Started Action %s by %s"), *GetNameSafe(this), *GetNameSafe(Instigator));

	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsActive = true;
}

void USActionBase::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped Action %s by %s"), *GetNameSafe(this), *GetNameSafe(Instigator));

	ensureAlways(IsActive());
	
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

bool USActionBase::IsActive() const
{
	return bIsActive;
}

UWorld* USActionBase::GetWorld() const
{
	const UActorComponent* OuterComponent = Cast<UActorComponent>(GetOuter());
	RETURN_VALUE_IF_NULL(OuterComponent, nullptr);
	return OuterComponent->GetWorld();
}

USActionComponent* USActionBase::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}