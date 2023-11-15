


#include "Character/SAnimInstance.h"

#include "Course.h"
#include "GameplayTagContainer.h"
#include "GAS/SActionComponent.h"

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AActor* OwningActor = GetOwningActor();
	RETURN_IF_NULL(OwningActor);

	ActionComponent = USActionComponent::Get(OwningActor);
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag(TEXT("State.Stunned"));

	RETURN_IF_NULL(ActionComponent);

	bIsStunned = ActionComponent->ActiveGameplayTags.HasTag(StunnedTag);

	
}
