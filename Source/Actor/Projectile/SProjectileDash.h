// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile/SProjectile.h"
#include "SProjectileDash.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDashFinished);

UCLASS()
class COURSE_API ASProjectileDash : public ASProjectile
{
	GENERATED_BODY()
public:

	ASProjectileDash();

protected:

	UPROPERTY(BlueprintAssignable)
	FOnDashFinished OnDashFinished;

	UPROPERTY(EditAnywhere, Category="Dash")
	float DashTime = 0.2f;

	void PerformDash(FVector Location = FVector::ZeroVector, FVector Normal = FVector::ZeroVector);

	virtual void BeginPlay() override;
	
	virtual void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;

private:
	
	FTimerHandle DashHandle;
};
