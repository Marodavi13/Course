// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "SCharacter.generated.h"

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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Movement functions*/
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);

};
