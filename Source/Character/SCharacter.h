// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile/SProjectile.h"
#include "Component/SAttributeComponent.h"
#include "GameFramework/Character.h"
#include "GAS/SActionComponent.h"

#include "SCharacter.generated.h"

class USInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

DECLARE_DELEGATE_OneParam(FPrimaryAttack, FName /**, Projectile Action Name*/ )

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

	virtual FVector GetPawnViewLocation() const override;
	
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);
	
protected:
	/** Spring arm that links the camera to the Character*/
	UPROPERTY(VisibleAnywhere, Category="Components")
	USpringArmComponent* SpringArmComponent;

	/** Character camera*/
	UPROPERTY(VisibleAnywhere, Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USActionComponent* ActionComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	FLinearColor DamageMaterialColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	float DamageMaterialSpeed = 4.f;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Bind the component delegates here*/
	virtual void PostInitializeComponents() override;

	void DrawDebug() const;

	/** Movement functions*/
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);

	/** Plays the montage that will perform the attack*/
	void LaunchProjectile(FName ActionName);
	
	/** Interacts with the closest Actor*/
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float DeltaHealth);

	UFUNCTION()
	void SprintStart();

	UFUNCTION()
	void SprintStop();
};
