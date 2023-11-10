// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Course.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Component/SAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"
#include "UI/SWorldUserWidget.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sensing Component"));
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attribute Component"));
	ActionComponent = CreateDefaultSubobject<USActionComponent>(TEXT("Action Component"));
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth,
                                    float Delta)
{
	RETURN_IF_TRUE(Delta >= 0.f);
	
	GetMesh()->SetScalarParameterValueOnMaterials(TimeOfHitParamName, GetWorld()->TimeSeconds);

	// Show the health widget if it has not been shown before
	if (!ActiveHealthBar)
	{
		ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->AddToViewport();
		}
	}
	
	if (InstigatorActor != this)
	{
		SetTargetActor(InstigatorActor);
	}

	// Is dead?
	RETURN_IF_FALSE(NewHealth <= 0.f);

	// Stop BT
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBrainComponent()->StopLogic(TEXT("Killed"));
	}
	// Ragdoll
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	
	// set lifespan
	SetLifeSpan(10.f);
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);

	RETURN_IF_NULL(TargetSpottedClass);
	
	ActiveTargetSpotted = CreateWidget<USWorldUserWidget>(GetWorld(), TargetSpottedClass);
	if (ActiveTargetSpotted)
	{
		ActiveTargetSpotted->AttachedActor = this;
	}
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	AActor* OldTarget = Cast<AActor>(BlackboardComponent->GetValueAsObject(TEXT("TargetActor")));
	if (OldTarget != NewTarget)
	{
		TargetActor = NewTarget;
		BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), NewTarget);
		OnTargetActorChanged.Broadcast(NewTarget, OldTarget);
	}
}

void ASAICharacter::OnPawnSeen(APawn* SeenPawn)
{
	SetTargetActor(SeenPawn);
}

void ASAICharacter::OnRep_TargetActor(AActor* OldTarget)
{
	OnTargetActorChanged.Broadcast(TargetActor, OldTarget);
}

void ASAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASAICharacter, TargetActor);
}
