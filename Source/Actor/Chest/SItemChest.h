// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Interface/SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class COURSE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual void OnActorLoaded_Implementation() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	
	UPROPERTY(VisibleAnywhere, Category="Mesh")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	UStaticMeshComponent* UpperMesh;

	UPROPERTY(Transient, SaveGame, Replicated, ReplicatedUsing = "OnRep_IsLidOpen")
	bool bIsLidOpen;

	UPROPERTY(EditAnywhere, Category="Interaction")
	float TargetPitch = 110.f;

	UFUNCTION()
	void OnRep_IsLidOpen();
	
};

