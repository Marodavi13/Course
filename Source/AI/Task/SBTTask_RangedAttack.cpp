// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Component/SAttributeComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/** Get AI controller, return if nullptr*/
	AAIController* AIController = OwnerComp.GetAIOwner();
	if(!ensure(AIController))
	{
		return EBTNodeResult::Failed;
	}
	
	/** Get AI character, return if nullptr*/
	ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
	if(!AICharacter)
	{
		return EBTNodeResult::Failed;
	}

	const FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation(TEXT("Muzzle_Front"));

	/** Get target actor, return if nullptr*/
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	/** Don't attack a dead target */
	if (!USAttributeComponent::IsActorAlive(TargetActor))
	{
		return EBTNodeResult::Failed;
 
	}
	
	const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();

	// Apply some deviation
	MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxBulletSpread);
	MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);
	
	/** Spawn the projectile */
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = AICharacter;
	
	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters);
	
	return SpawnedProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
