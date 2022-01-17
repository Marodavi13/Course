// Copyright Epic Games, Inc. All Rights Reserved.


#include "SGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "AI/SAICharacter.h"
#include "Character/Component/SAttributeComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"

ASGameModeBase::ASGameModeBase()
{
	SpawnBotsDelay = 2.f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(SpawnBotsHandle, this, &ASGameModeBase::SpawnBots, SpawnBotsDelay, true);
}

void ASGameModeBase::SpawnBots()
{
	if(ensure(SpawnBotQuery))
	{
		int32 NumberOfBots = 0;
		for(TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
		{
			ASAICharacter* Bot = *It;
			USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Bot);
			if(AttributeComponent && AttributeComponent->IsAlive())
			{
				++NumberOfBots;
	
			}
		}

		UE_LOG(LogTemp, Log, TEXT("There are %i enemies alive"), NumberOfBots);
	
		float MaxBotCount = 10.f;
		if (DifficultyCurve)
		{
			MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
		}
	
		if(NumberOfBots >= MaxBotCount)
		{
			UE_LOG(LogTemp, Log, TEXT("There are too many anemies. Skipping bot spawn"), NumberOfBots);
			return;
		}
		
		UEnvQueryInstanceBlueprintWrapper* Wrapper = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
		Wrapper->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
		
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if(Locations.Num())
	{
		GetWorld()->SpawnActor<AActor>(BotClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugLine(GetWorld(), Locations[0], Locations[0] + FVector::UpVector * 500.f, FColor::Green,
		false, 10.f);
	}
	
}
