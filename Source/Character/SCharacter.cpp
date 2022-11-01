// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"



#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Component/SAttributeComponent.h"
#include "Component/SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>(TEXT("Interaction Component"));

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attribute Component"));
	bUseControllerRotationYaw = false;

	ActionComponent = CreateDefaultSubobject<USActionComponent>(TEXT("Action Component"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::DrawDebug() const
{
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	const FVector LineStart = GetActorLocation();

	// Set line end in direction of the actor's forward
	const FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow,
	                          false, 0.0f, (uint8)SDPG_Foreground, Thickness);

	const FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green,
	                          false, 0.0f, (uint8)SDPG_Foreground, Thickness);
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebug();
}

void ASCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComponent->ApplyHealthChange(Amount, this);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;

	AddMovementInput(FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y), Value);
}

void ASCharacter::LaunchProjectile(FName ActionName)
{
	ActionComponent->StartActionByName(this, ActionName);
}

void ASCharacter::PrimaryInteract()
{
	InteractionComponent->Interact();
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth,
	float DeltaHealth)
{

	if(DeltaHealth < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TEXT("TimeOfHit"), GetWorld()->TimeSeconds);
	}
	
	if(NewHealth < 0.f && DeltaHealth < 0.f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
		SetActorEnableCollision(false);
	}
}

void ASCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, TEXT("Sprint"));
}

void ASCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, TEXT("Sprint"));
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);

}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ASCharacter::AddControllerPitchInput);

	FName MagicProjectileName = TEXT("MagicProjectile");
	FName BlackHoleProjectileName = TEXT("BlackHoleProjectile");
	FName DashProjectileName = TEXT("DashProjectile");
	PlayerInputComponent->BindAction<FPrimaryAttack>(MagicProjectileName, IE_Pressed, this,
		&ASCharacter::LaunchProjectile, MagicProjectileName);
	PlayerInputComponent->BindAction<FPrimaryAttack>(BlackHoleProjectileName, IE_Pressed, this,
		&ASCharacter::LaunchProjectile, BlackHoleProjectileName);
	PlayerInputComponent->BindAction<FPrimaryAttack>(DashProjectileName, IE_Pressed, this,
		&ASCharacter::LaunchProjectile, DashProjectileName);
	
	PlayerInputComponent->BindAction(TEXT("PrimaryInteract"), IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ASCharacter::SprintStop);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}


