// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;
class UCurveFloat;

UCLASS()
class COURSE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameModeBase();
	
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAllAI();

	virtual void OnActorKilled(AActor* KilledActor, AActor* KillInstigator);
protected:
	
	UPROPERTY(EditAnywhere, Category="AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditAnywhere, Category="AI")
	TSubclassOf<AActor> BotClass;

	UPROPERTY(EditAnywhere, Category="AI")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditAnywhere, Category="AI")
	float SpawnBotsDelay;

	
	FTimerHandle SpawnBotsHandle;

	virtual void SpawnBots();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* RespawningController);

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type Arg);
	
};
