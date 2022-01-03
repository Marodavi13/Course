// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile/SProjectile.h"
#include "Component/SAttributeComponent.h"
#include "GameFramework/Character.h"

#include "SCharacter.generated.h"

class USInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

DECLARE_DELEGATE_OneParam(FPrimaryAttack, int32 /**, Projectile Class index*/ )

UCLASS()
class COURSE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Spring arm that links the camera to the Character*/
	UPROPERTY(VisibleAnywhere, Category="Components")
	USpringArmComponent* SpringArmComponent;

	/** Character camera*/
	UPROPERTY(VisibleAnywhere, Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComponent;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnimation;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	TArray<TSubclassOf<ASProjectile>> ProjectileClasses;
		
	FTimerHandle TimerHandlePrimaryAttack;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void DrawDebug() const;

	/** Movement functions*/
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);

	/** Plays the montage that will perform the attack*/
	void LaunchProjectile(int32 Index);

	/** Performs the attack, shooting a projectile from its hand*/
	void PerformLaunchProjectile(int32 Index);


	/** Interacts with the closest Actor*/
	void PrimaryInteract();

};
