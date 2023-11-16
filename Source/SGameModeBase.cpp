// Copyright Epic Games, Inc. All Rights Reserved.


#include "SGameModeBase.h"

#include "Course.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "SkeletalDebugRendering.h"
#include "AI/SAICharacter.h"
#include "AI/Data/SBotData.h"
#include "Character/SCharacter.h"
#include "Character/SPlayerState.h"
#include "Character/Component/SAttributeComponent.h"
#include "Core/SSaveGame.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(
	TEXT("su.SpawnBots"),
	false,
	TEXT("Enable Spawning of bots"),
	ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnBotsDelay = 2.f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(SpawnBotsHandle, this, &ASGameModeBase::SpawnBots, SpawnBotsDelay, true);

	for (int32 i = 0; i < InitialCoins; ++i)
	{
		SpawnCoin();
	}
	
	GetWorldTimerManager().SetTimer(SpawnCoinsHandle, this, &ASGameModeBase::SpawnCoin, SpawnCoinsDelay, true);
}

void ASGameModeBase::KillAllAI()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComponent && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this);
		}
	}
}

void ASGameModeBase::OnActorKilled(AActor* KilledActor, AActor* KillInstigator)
{
	ASCharacter* Player = Cast<ASCharacter>(KilledActor);
	if (Player)
	{
		FTimerHandle RespawnHandle;
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &ASGameModeBase::RespawnPlayerElapsed, Player->GetController());

		float RespawnDelay = 2.f;
		UE_LOG(LogTemp, Log, TEXT("Respawning character %s in %i seconds"), *GetNameSafe(Player), RespawnDelay);
		GetWorldTimerManager().SetTimer(RespawnHandle, Delegate, RespawnDelay, false);
	}

	OnActorKilledDelegate.Broadcast(KilledActor, KillInstigator);
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadGame();
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	RETURN_IF_NULL_ENSURE(PlayerState);

	PlayerState->LoadPlayerState(CurrentSavedGame);

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ASGameModeBase::SpawnBots()
{
	RETURN_IF_FALSE(CVarSpawnBots.GetValueOnGameThread());
	ensure(SpawnBotQuery);
	RETURN_IF_NULL(SpawnBotQuery);
	
	int32 NumberOfBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComponent && AttributeComponent->IsAlive())
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

	if (NumberOfBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("There are too many anemies. Skipping bot spawn"), NumberOfBots);
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* Wrapper = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	Wrapper->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotQueryCompleted);
}

void ASGameModeBase::RespawnPlayerElapsed(AController* RespawningController)
{
	ensure(RespawningController);
	RespawningController->UnPossess();
	RestartPlayer(RespawningController);
}


void ASGameModeBase::OnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
		
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query has no locations!"));
		return;
	}
	
	RETURN_IF_NULL_ENSURE(BotTable)
	
	TArray<FSBotInfoRow*> OutEnemies;
	BotTable->GetAllRows(TEXT("Spawn Enemy"), OutEnemies);

	int32 RandomIndex = FMath::RandRange(0, OutEnemies.Num() - 1);
	FSBotInfoRow* SelectedEnemy = OutEnemies[RandomIndex];

	UAssetManager* Manager = UAssetManager::GetIfInitialized();
	RETURN_IF_NULL_ENSURE(Manager);

	TArray<FName> Bundles;
	FStreamableDelegate LoadDelegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnBotLoaded, SelectedEnemy->BotID,Locations[0]);
	Manager->LoadPrimaryAsset(SelectedEnemy->BotID, Bundles, LoadDelegate);
	LogOnScreen(this, TEXT("Loading Bot..."));

}

void ASGameModeBase::OnBotLoaded(FPrimaryAssetId PrimaryAssetId, FVector SpawnLocation)
{
	LogOnScreen(this, TEXT("Bot Loaded!"));

	UAssetManager* Manager = UAssetManager::GetIfInitialized();
	RETURN_IF_NULL_ENSURE(Manager);

	USBotData* BotData = Manager->GetPrimaryAssetObject<USBotData>(PrimaryAssetId);
	AActor* NewBot = GetWorld()->SpawnActor<AActor>(BotData->BotClass, SpawnLocation, FRotator::ZeroRotator);
	RETURN_IF_NULL_ENSURE(NewBot);

	LogOnScreen(this, TEXT("Bot Spawned!"));

	USActionComponent* ActionComponent = USActionComponent::Get(NewBot);
	RETURN_IF_NULL(ActionComponent);
	
	for (auto Action : BotData->Actions)
	{
		ActionComponent->AddAction(NewBot, Action);
	}
}

void ASGameModeBase::OnCoinPickedUp(AActor* Actor)
{
	CoinInstances.Remove(Cast<ASPickUpCoin>(Actor));
}

void ASGameModeBase::OnCoinQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                          EEnvQueryStatus::Type QueryStatus)
{
	RETURN_IF_FALSE(QueryStatus == EEnvQueryStatus::Success)

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	ASPickUpCoin* SpawnedCoin = GetWorld()->SpawnActor<ASPickUpCoin>(CoinClass, Locations[0], FRotator::ZeroRotator);

	SpawnedCoin->OnDestroyed.AddUniqueDynamic(this, &ASGameModeBase::OnCoinPickedUp);
	CoinInstances.Add(SpawnedCoin);
}

void ASGameModeBase::SpawnCoin()
{
	ensure(SpawnCoinQuery);
	RETURN_IF_NULL(SpawnCoinQuery);
	ensure(CoinClass);
	RETURN_IF_NULL(CoinClass);

	RETURN_IF_TRUE(CoinInstances.Num() >= MaxNumberOfCoins);

	UEnvQueryInstanceBlueprintWrapper* Wrapper = UEnvQueryManager::RunEQSQuery(this, SpawnCoinQuery, this, EEnvQueryRunMode::RandomBest25Pct, nullptr);
	Wrapper->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnCoinQueryCompleted);
}

void ASGameModeBase::DisableAI()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, BotClass, OutActors);

	for (AActor* Actor : OutActors)
	{
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Actor);
		AttributeComp->Kill(this);
	}

	CVarSpawnBots->SetWithCurrentPriority(false);
}

static FString SlotName = TEXT("SavedGame");

void ASGameModeBase::EnableAI()
{
	CVarSpawnBots->SetWithCurrentPriority(true);
}


void ASGameModeBase::SaveGame()
{
	for (APlayerState* PS : GameState->PlayerArray)
	{
		ASPlayerState* CurrentPlayerState = Cast<ASPlayerState>(PS);
		if (CurrentPlayerState)
		{
			CurrentPlayerState->SavePlayerState(CurrentSavedGame);
			// SP only by now
			break;
		}
	}

	CurrentSavedGame->SavedActors.Empty();
	
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData NewData;
		NewData.ActorName = Actor->GetName();
		NewData.ActorTransform = Actor->GetTransform();

		// Pass the array to fill with data from actor
		FMemoryWriter MemWriter(NewData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);

		//Find only variable with UPROPERTY SaveGame
		Ar.ArIsSaveGame = true;

		//Converts these UPROPERTIES into bin array 
		Actor->Serialize(Ar);
		CurrentSavedGame->SavedActors.Add(NewData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSavedGame, SlotName, 0);
}

void ASGameModeBase::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSavedGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (!CurrentSavedGame)
		{
			UE_LOG(LogSSaveGame, Warning, TEXT("Failed to load Save Game Data"));
			return;
		}

		UE_LOG(LogSSaveGame, Log, TEXT("Loaded Save Game Data"));

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (const FActorSaveData& ActorData : CurrentSavedGame->SavedActors)
			{
				if (ActorData.ActorName != Actor->GetName())
				{
					continue;
				}

				Actor->SetActorTransform(ActorData.ActorTransform);

				FMemoryReader MemReader(ActorData.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);

				Ar.ArIsSaveGame = true;

				//Converts bin array back to actor's variables
				Actor->Serialize(Ar);
				ISGameplayInterface::Execute_OnActorLoaded(Actor);
				break;
			}
		}
	}
	else
	{
		CurrentSavedGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogSSaveGame, Log, TEXT("Created new Save Game Data"));
	}


}
