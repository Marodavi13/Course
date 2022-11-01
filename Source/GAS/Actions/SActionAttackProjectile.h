// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Actions/SActionBase.h"
#include "SActionAttackProjectile.generated.h"

class ASProjectile;
/**
 * 
 */
UCLASS()
class COURSE_API USActionAttackProjectile : public USActionBase
{
	GENERATED_BODY()
public:

	USActionAttackProjectile();

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(VisibleAnywhere, Category="Attack")
	FName AttackBoneName = TEXT("Muzzle_01");

	UPROPERTY(VisibleAnywhere, Category="Attack")
	FName TimeToHitParamName = TEXT("TimeToHit");
	
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnimation = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<ASProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	UParticleSystem* LaunchProjectileEffect;

	FTimerHandle ProjectileAttackHandle;

	UFUNCTION()
	void OnAttackDelayElapsed(ACharacter* Instigator);
	void LaunchProjectile(ACharacter* Instigator);
};
