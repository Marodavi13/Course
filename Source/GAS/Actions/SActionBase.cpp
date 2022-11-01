// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Actions/SActionBase.h"

#include "Course.h"

void USActionBase::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Started Action %s by %s"), *GetNameSafe(this), *GetNameSafe(Instigator));
}

void USActionBase::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped Action %s by %s"), *GetNameSafe(this), *GetNameSafe(Instigator));
}

UWorld* USActionBase::GetWorld() const
{
	const UActorComponent* OuterComponent = Cast<UActorComponent>(GetOuter());
	RETURN_VALUE_IF_NULL(OuterComponent, nullptr);
	return OuterComponent->GetWorld();
}
