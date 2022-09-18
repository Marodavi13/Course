// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Actions/SActionBase.h"

void USActionBase::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Started Action %s by %s"), *GetNameSafe(this), *GetNameSafe(Instigator));
}

void USActionBase::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped Action %s by %s"), *GetNameSafe(this), *GetNameSafe(Instigator));
}
