// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Actor/Projectile/SProjectile.h"
#include "SProjectileMagic.generated.h"

class USActionEffectBase;
/**
 * 
 */
UCLASS()
class COURSE_API ASProjectileMagic : public ASProjectile
{
	GENERATED_BODY()

public:
protected:
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float Damage = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<USActionEffectBase> OnDamageEffect;
	
	bool bHasBeenParried = false;
	
	virtual void BeginPlay() override;

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
