// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/SInteractionComponent.h"

#include "Course.h"
#include "ToolContextInterfaces.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actor/Interface/SGameplayInterface.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Curves/CurveLinearColor.h"
#include "UI/SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("su.Draw.Interaction"),
	false,
	TEXT("Draws interaction debug shapes"),
	ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USInteractionComponent::UpdateInteractWidget()
{
	if (FocusedActor)
	{
		if(!InteractWidget && ensure(InteractWidgetClass))
		{
			InteractWidget = CreateWidget<USWorldUserWidget>(GetWorld(), InteractWidgetClass);
		}

		if (InteractWidget)
		{
			InteractWidget->AttachedActor = FocusedActor;
			
			if(!InteractWidget->IsInViewport())
			{
				InteractWidget->AddToViewport();
			}
		}
	}
	else
	{
		if(InteractWidget)
		{
			InteractWidget->RemoveFromParent();
		}
	}
}

void USInteractionComponent::FindBestInteractable()
{
	/** Trace from eye location to where I am looking*/
	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	const FVector End = EyeLocation + EyeRotation.Vector() * InteractionDistance;

	/** Initialize other variables*/
	TArray<FHitResult> Hits;
	const TArray<AActor*> ActorsToIgnore;

	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(CollisionChannel);
	FCollisionShape Shape;
	Shape.SetSphere(InteractionRadius);
	
	/** Perform the sphere trace */
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, QueryParams, Shape);


	FColor LineColor = bBlockingHit? FColor::Green : FColor::Red;
	bool bDrawDebug = CVarDebugDrawInteraction.GetValueOnGameThread();

	//Clear ref before setting it again
	FocusedActor= nullptr;
	
	/** Search a Interactable interface and interact with the first result*/
	for (const FHitResult& Hit: Hits)
	{
		if (bDrawDebug)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, InteractionRadius, 32, LineColor, false, 2.f);
		}
		
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<USGameplayInterface>())
		{
			FocusedActor = Hit.GetActor();
			break;
		}
	}
	
	UpdateInteractWidget();
	
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.f,0, 2.f);
	}
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

	APawn* OwnerPawn = GetOwner<APawn>();
	RETURN_IF_NULL(OwnerPawn);
	RETURN_IF_FALSE(OwnerPawn->IsLocallyControlled())

	FindBestInteractable();
}

void USInteractionComponent::Server_Interact_Implementation(AActor* InteractedActor)
{
	if (InteractedActor)
	{
		ISGameplayInterface::Execute_Interact(InteractedActor, Cast<APawn>(GetOwner()));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Not focused actor to interact with"));
	}
}

void USInteractionComponent::Interact()
{
	Server_Interact(FocusedActor);
}

