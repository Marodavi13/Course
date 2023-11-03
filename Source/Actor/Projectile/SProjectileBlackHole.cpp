// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/SProjectileBlackHole.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

void ASProjectileBlackHole::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ShrinkingActors.Num())
	{
		TArray<AActor*> ActorsToRemove;
		for(AActor* ShrinkingActor : ShrinkingActors)
		{
			
			ShrinkingActor->SetActorScale3D(ShrinkingActor->GetActorScale3D() - ShrinkSpeed * DeltaSeconds);
			FVector CurrentScale = ShrinkingActor->GetActorScale3D();

			if (CurrentScale.GetMin() <= 0.f)
			{
				ActorsToRemove.Add(ShrinkingActor);
			}
		}

		for(AActor* ShrunkActor : ActorsToRemove)
		{
			ShrinkingActors.Remove(ShrunkActor);
			ShrunkActor->Destroy();
		}

		if (ShrinkingActors.Num() == 0 && bIsWaitingToBeDestroyed)
		{
			Destroy();
		}
	}
}

void ASProjectileBlackHole::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(OtherActor != GetInstigator());
	
	ShrinkingActors.Add(OtherActor);
}

void ASProjectileBlackHole::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	EffectComponent->OnSystemFinished.AddDynamic(this, &ThisClass::OnBlackHoleEffectFinish);
}

void ASProjectileBlackHole::OnBlackHoleEffectFinish(UParticleSystemComponent* ComponentFinished)
{
	/** If I have shrinking actors, fast forward them until I have none and I can be destroyed*/
	if (ShrinkingActors.Num())
	{
		bIsWaitingToBeDestroyed = true;
		ShrinkSpeed *= 4.f;
		SphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ASProjectileBlackHole::OnSphereBeginOverlap);
	}
	else
	{
		Destroy();
	}
}
