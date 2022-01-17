// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/SInteractionComponent.h"
#include "Kismet/KismetSystemLibrary.h"





#include "DrawDebugHelpers.h"
#include "Actor/Interface/SGameplayInterface.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::Interact() const
{
	/** Trace from eye location to where I am looking*/
	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	FVector CameraLocation = Camera->GetComponentLocation();
	FVector CenteredEyeLocation = EyeLocation;


	float Dist = ((EyeLocation - CameraLocation) * Camera->GetForwardVector()).Size();
	
	CenteredEyeLocation = CameraLocation + Dist * Camera->GetForwardVector();

	const FVector End = CenteredEyeLocation + EyeRotation.Vector() * InteractionDistance;

	/** Initialize other variables*/
	TArray<FHitResult> Hits;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectQuery;
	ObjectQuery.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	const TArray<AActor*> ActorsToIgnore;

	/** Perform the sphere trace */
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), EyeLocation, End, 25.f, ObjectQuery,false,
	                                                 ActorsToIgnore,EDrawDebugTrace::ForDuration,Hits, true);

	/** Search a Interactable interface and interact with the first result*/
	for(const FHitResult& Hit: Hits)
	{
		if(AActor* HitActor = Hit.GetActor())
		{
			if(HitActor->Implements<USGameplayInterface>())
			{
				ISGameplayInterface::Execute_Interact(HitActor, Cast<APawn>(GetOwner()));
				break;
			}
		}
	}
}

