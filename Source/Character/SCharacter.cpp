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

// ReSharper disable once CppMemberFunctionMayBeConst
void ASCharacter::LaunchProjectile(int32 Index)
{
	if(ensureMsgf(ProjectileClasses.IsValidIndex(Index), TEXT("Projectile Classes Index %i not valid"), Index))
	{
		PlayAnimMontage(AttackAnimation);
		
		if(LaunchProjectileEffects.IsValidIndex(Index))
		{
			UGameplayStatics::SpawnEmitterAttached(LaunchProjectileEffects[Index],GetMesh(), AttackBoneName);
		}
			
		const FTimerDelegate Delegate  = FTimerDelegate::CreateUObject(this, &ASCharacter::PerformLaunchProjectile, Index);
		GetWorldTimerManager().SetTimer(TimerHandlePrimaryAttack, Delegate, 0.17, false);
	}	
}

void ASCharacter::PerformLaunchProjectile(int32 Index)
{
	/** Set locations */
	const FVector HandLocation = GetMesh()->GetSocketLocation(AttackBoneName);
	const FVector StartLocation = CameraComponent->GetComponentLocation();
	FVector EndLocation = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 10000.f;

	/** Set Line Trace variables*/
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	FCollisionQueryParams Params;
	
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	
	Params.AddIgnoredActor(this);
	Params.bDebugQuery = true;

	/**  Perform Line trace from camera */
	bool bHit = GetWorld()->LineTraceSingleByObjectType(Hit, StartLocation, EndLocation, ObjectQueryParams, Params);

	if (bHit)
	{
		EndLocation = Hit.ImpactPoint;
	}
	// DrawDebugDirectionalArrow(GetWorld(), HandLocation, EndLocation, 5.f, bHit ? FColor::Green : FColor::Red,
	// 	false, 2.f, 0, 3.f);

	/**  Set the transform based on the Hit Result*/
	const FTransform SpawnTransform = FTransform((EndLocation - HandLocation).Rotation(), HandLocation);

	/** Spawn the projectile */
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;

	if(!ProjectileClasses.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile classes in %s doesn't hav valid index %i"), *GetNameSafe(this), Index);
		return;
	}

	GetWorld()->SpawnActor<AActor>(ProjectileClasses[Index], SpawnTransform, SpawnParameters);
	
}

void ASCharacter::PrimaryInteract()
{
	InteractionComponent->Interact();
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth,
	float DeltaHealth)
{
	if(NewHealth < 0.f && DeltaHealth < 0.f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
		SetActorEnableCollision(false);
	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);

}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ASCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction<FPrimaryAttack>(TEXT("MagicProjectile"), IE_Pressed, this,
		&ASCharacter::LaunchProjectile, 0);
	PlayerInputComponent->BindAction<FPrimaryAttack>(TEXT("BlackHoleProjectile"), IE_Pressed, this,
		&ASCharacter::LaunchProjectile, 1);
	PlayerInputComponent->BindAction<FPrimaryAttack>(TEXT("DashProjectile"), IE_Pressed, this,
		&ASCharacter::LaunchProjectile, 2);
	
	PlayerInputComponent->BindAction(TEXT("PrimaryInteract"), IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ASCharacter::Jump);
}

