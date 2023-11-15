// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Actor/PickUpActor/SPickUpCoin.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USBotData;
class USSaveGame;
class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FSBotInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USBotData* BotData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 KillReward = 15.f;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorKilledDelegate, AActor*, KilledActor, AActor*, KillInstigator);
UCLASS()
class COURSE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	// Delegates
	UPROPERTY(BlueprintAssignable, Category="Game")
	FOnActorKilledDelegate OnActorKilledDelegate;
	FTimerHandle SpawnCoinsHandle;

	ASGameModeBase();
	
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAllAI();

	virtual void OnActorKilled(AActor* KilledActor, AActor* KillInstigator);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void SaveGame();

	void LoadGame();
	
protected:

	UPROPERTY(EditAnywhere, Category="Credits")
	UEnvQuery* SpawnCoinQuery;

	UPROPERTY(EditAnywhere, Category="Credits")
	int32 InitialCoins = 4;

	UPROPERTY(EditAnywhere, Category="Credits")
	int32 MaxNumberOfCoins = 8;

	UPROPERTY(EditAnywhere, Category="Credits")
	float SpawnCoinsDelay = 10.f;

	UPROPERTY(EditAnywhere, Category="Credits")
	TSubclassOf<ASPickUpCoin> CoinClass;

	TArray<TWeakObjectPtr<ASPickUpCoin>> CoinInstances;
	
	UPROPERTY(EditAnywhere, Category="AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditAnywhere, Category="AI")
	UDataTable* BotTable;

	UPROPERTY(EditAnywhere, Category="AI")
	TSubclassOf<AActor> BotClass;

	UPROPERTY(EditAnywhere, Category="AI")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditAnywhere, Category="AI")
	float SpawnBotsDelay;
	
	UPROPERTY()
	USSaveGame* CurrentSavedGame;

	FTimerHandle SpawnBotsHandle;

	virtual void SpawnBots();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* RespawningController);

	UFUNCTION()
	void OnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type Arg);

	UFUNCTION()
	void OnCoinPickedUp(AActor* Actor);
	UFUNCTION()
	void OnCoinQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	void SpawnCoin();
private:

	UFUNCTION(Exec)
	void DisableAI();
	UFUNCTION(Exec)
	void EnableAI();

};
