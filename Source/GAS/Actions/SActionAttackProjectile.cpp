// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Actions/SActionAttackProjectile.h"

#include "Course.h"
#include "Actor/Projectile/SProjectile.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USActionAttackProjectile::USActionAttackProjectile()
{
}

void USActionAttackProjectile::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// if our instigator is not a character, exit
	ACharacter* CharacterInstigator = Cast<ACharacter>(Instigator);
	RETURN_IF_NULL(CharacterInstigator)

	// Start the Anim Montage and the VFX
	CharacterInstigator->PlayAnimMontage(AttackAnimation);
	UGameplayStatics::SpawnEmitterAttached(LaunchProjectileEffect,CharacterInstigator->GetMesh(), AttackBoneName);

	// Set the timer for the anim delay
	const FTimerDelegate Delegate  = FTimerDelegate::CreateUObject(this, &USActionAttackProjectile::OnAttackDelayElapsed,
																	     CharacterInstigator);
	GetWorld()->GetTimerManager().SetTimer(ProjectileAttackHandle, Delegate, 0.17f, false);
}

void USActionAttackProjectile::OnAttackDelayElapsed(ACharacter* Instigator)
{
	LaunchProjectile(Instigator);
}

void USActionAttackProjectile::LaunchProjectile(ACharacter* Instigator)
{
	if(!Instigator || !ProjectileClass)
	{
		ensure(false);
		StopAction(Instigator);
	}

	/** Set locations */
	const FVector HandLocation = Instigator->GetMesh()->GetSocketLocation(AttackBoneName);
	const FVector StartLocation = Instigator->GetPawnViewLocation();
	FVector EndLocation = StartLocation  + Instigator->GetControlRotation().Vector() * 10000.f;

	/** Set Line Trace variables*/
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	FCollisionQueryParams Params;
	
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	
	Params.AddIgnoredActor(Instigator);
	Params.bDebugQuery = true;

	/**  Perform Line trace from camera */
	bool bHit = GetWorld()->LineTraceSingleByObjectType(Hit, StartLocation, EndLocation, ObjectQueryParams, Params);

	if (bHit)
	{
		EndLocation = Hit.ImpactPoint;
	}
	// DrawDebugDirectionalArrow(GetWorld(), HandLocation, EndLocation, 5.f, bHit ? FColor::Green : FColor::Red,
	// 	false, 2.f, 0, 3.f);

	/**  Set the transform based on the Hit Result*/
	const FTransform SpawnTransform = FTransform((EndLocation - HandLocation).Rotation(), HandLocation);

	/** Spawn the projectile */
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = Instigator;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParameters);
}
