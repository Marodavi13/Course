// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPlayerState.h"

#include "Course.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

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

void ASPlayerState::AddCredits(int32 AddedCredits, FName Reason)
{
	ensure(AddedCredits >= 0.f);
	ensure(HasAuthority());
	RETURN_IF_FALSE(HasAuthority());
		
	FSCreditTransaction NewTransaction;
	NewTransaction.TransactionAmount = AddedCredits;
	NewTransaction.TimeOfTransaction = GetWorld()->GetTimeSeconds();
	NewTransaction.Reason = Reason;

	DoCreditTransaction(NewTransaction);
}

bool ASPlayerState::RemoveCredits(int32 RemovedCredits, FName Reason)
{
	ensure(RemovedCredits >= 0.f);
	ensure(HasAuthority());
	RETURN_VALUE_IF_FALSE(HasAuthority(), false);

	FSCreditTransaction NewTransaction;
	NewTransaction.TransactionAmount = -RemovedCredits;
	NewTransaction.TimeOfTransaction = GetWorld()->GetTimeSeconds();
	NewTransaction.Reason = Reason;
	
	return DoCreditTransaction(NewTransaction);
}

bool ASPlayerState::DoCreditTransaction(FSCreditTransaction Transaction)
{
	if (Transaction.TransactionAmount < 0 && PlayerCredits < Transaction.TransactionAmount)
	{
		Transaction.bWasTransactionSuccessful = false;
		UE_LOG(LogSCredits, Log, TEXT("Actor %s tried to remove %i credits but he doesn't have enough!. Total Credits: %i"),
								  *GetNameSafe(GetPawn()), -Transaction.TransactionAmount, PlayerCredits);
		Transactions.Add(Transaction);
		return false;
	}
	float OldCredits = PlayerCredits;
	Transaction.bWasTransactionSuccessful = true;
	PlayerCredits += Transaction.TransactionAmount;
	Transactions.Add(Transaction);

	OnPlayerCreditsUpdated.Broadcast(PlayerCredits, OldCredits);

	UE_LOG(LogSCredits, Log, TEXT("Actor %s has done a credit transaction of %i credits due %s. Total Credits: %i"),
		*GetNameSafe(GetPawn()), Transaction.TransactionAmount, *Transaction.Reason.ToString(), PlayerCredits);

	return true;
}

bool ASPlayerState::CanRemoveCredits(int32 RemovedCredits) const
{
	ensure(RemovedCredits >= 0.f);

	return PlayerCredits >= RemovedCredits;
}

void ASPlayerState::OnRep_PlayerCredits(int32 OldValue)
{
	OnPlayerCreditsUpdated.Broadcast(PlayerCredits, OldValue);
}

void ASPlayerState::OnActorKilled(AActor* KilledActor, AActor* KillInstigator)
{
	RETURN_IF_FALSE(KillInstigator == GetPawn());

	if(HasAuthority())
	{
		AddCredits(CreditsPerKill, TEXT("Actor Killed"));
	}
}

// REPLICATION FUNCS
void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASPlayerState, PlayerCredits);
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SavedGame)
{
	RETURN_IF_NULL(SavedGame);

	PlayerCredits = SavedGame->Credits;
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SavedGame)
{
	RETURN_IF_NULL(SavedGame);

	SavedGame->Credits = PlayerCredits;
}
