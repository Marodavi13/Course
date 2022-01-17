// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Component/SAttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "UI/SWorldUserWidget.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sensing Component"));

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attribute Component"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth,
	float Delta)
{
	if (Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeOfHitParamName, GetWorld()->TimeSeconds);

		if(!ActiveHealthBar)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if(ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
		if(InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
		
		if(NewHealth <= 0.f)
		{
			// Stop BT
			AAIController* AIController = Cast<AAIController>(GetController());
			if(AIController)
			{
				AIController->GetBrainComponent()->StopLogic(TEXT("Killed"));
			}
			// Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
			// set lifespan
			SetLifeSpan(10.f);
		}
	}
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if(!AIController)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if(BlackboardComponent->GetValueAsObject(TEXT("TargetActor")) != NewTarget)
	{
		BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), NewTarget);
		DrawDebugString(GetWorld(), GetActorLocation(), TEXT("I have detected the player!"),nullptr, FColor::White, 2.f);
	}
}

void ASAICharacter::OnPawnSeen(APawn* SeenPawn)
{
	SetTargetActor(SeenPawn);
}
