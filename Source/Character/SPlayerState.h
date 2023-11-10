// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

USTRUCT(BlueprintType)
struct FSCreditTransaction
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Transaction")
	int32 TransactionAmount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Transaction")
	FName Reason = TEXT("Unknown reason");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Transaction")
	float TimeOfTransaction = -1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Transaction")
	bool bWasTransactionSuccessful = true;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOnCreditTransactionDone, int32, NewAmount,  int32, OldAmount);

UCLASS()
class COURSE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FSOnCreditTransactionDone OnPlayerCreditsUpdated;
	
	// Sets default values for this actor's properties
	ASPlayerState();
		
	virtual void PostInitializeComponents() override;
	
	int32 GetPlayerCredits() const { return PlayerCredits; }

	// Does a credit transaction of delta quantity of credits
	UFUNCTION(BlueprintCallable, Category="Credits")
	void AddCredits(int32 AddedCredits, FName Reason = TEXT("Unnespeciffic transaction"));

	UFUNCTION(BlueprintCallable, Category="Credits")
	bool RemoveCredits(int32 RemovedCredits, FName Reason = TEXT("Unnespeciffic transaction"));
	
	bool CanRemoveCredits(int32 RemovedCredits) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	
	UPROPERTY(EditAnywhere, Category="Credits")
	int32 CreditsPerKill = 10;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, ReplicatedUsing="OnRep_PlayerCredits", Category="Credits")
	int32 PlayerCredits = 0;

	UFUNCTION()
	void OnRep_PlayerCredits(int32 OldValue);
	
	UFUNCTION()
	void OnActorKilled(AActor* KilledActor, AActor* KillInstigator);
	
private:

	bool DoCreditTransaction(FSCreditTransaction Transaction);
	
	TArray<FSCreditTransaction> Transactions;
};