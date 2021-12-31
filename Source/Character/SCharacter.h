// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile/SMagicProjectile.h"
#include "GameFramework/Character.h"

#include "SCharacter.generated.h"

class USInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
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

	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<ASMagicProjectile> ProjectileClass;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void DrawDebug() const;

	/** Movement functions*/
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);

	void PerformPrimaryAttack();
	void PerformPrimaryInteract();

};
