// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPlayerState.h"

#include "Course.h"
#include "SGameModeBase.h"

// Sets default values
ASPlayerState::ASPlayerState()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (GetWorld()->GetAuthGameMode<ASGameModeBase>())
	{
		GetWorld()->GetAuthGameMode<ASGameModeBase>()->OnActorKilledDelegate.AddDynamic(this, &ASPlayerState::OnActorKilled);
	}
}

bool ASPlayerState::DoCreditTransaction(int32 DeltaCredits, bool bDoBottomCheck, FName Reason)
{
	FSCreditTransaction NewTransaction;
	NewTransaction.TransactionAmount = DeltaCredits;
	NewTransaction.TimeOfTransaction = GetWorld()->GetTimeSeconds();
	NewTransaction.Reason = Reason;

	if (bDoBottomCheck && DeltaCredits < 0 && PlayerCredits < DeltaCredits)
	{
		NewTransaction.bWasTransactionSuccessful = false;
		UE_LOG(LogSCredits, Log, TEXT("Actor %s tried to remove %i credits but he doesn't have enough!. Total Credits: %i"),
									  *GetNameSafe(GetPawn()), -DeltaCredits, PlayerCredits);
	}
	else
	{
		NewTransaction.bWasTransactionSuccessful = true;
		PlayerCredits += DeltaCredits;
		
		Transactions.Add(NewTransaction);

		UE_LOG(LogSCredits, Log, TEXT("Actor %s has done a transaction of %i creadits due %s. Total Credits: %i"),
			*GetNameSafe(GetPawn()), DeltaCredits, *Reason.ToString(), PlayerCredits);

		OnCreditTransactionDone.Broadcast(NewTransaction);
	}
	
	return NewTransaction.bWasTransactionSuccessful;

}

bool ASPlayerState::CanDoCreditTransaction(int32 DeltaCredits) const
{
	return DeltaCredits >= 0.f || PlayerCredits + DeltaCredits >= 0 ;
}


// Called when the game starts or when spawned
void ASPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPlayerState::OnActorKilled(AActor* KilledActor, AActor* KillInstigator)
{
	RETURN_IF_FALSE(KillInstigator == GetPawn());
	DoCreditTransaction(CreditsPerKill, false, TEXT("Actor Killed"));

	
}

// Called every frame
void ASPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

