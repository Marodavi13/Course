


#include "Subsystem/SSaveGameSubsystem.h"

#include "Course.h"
#include "EngineUtils.h"
#include "Actor/Interface/SGameplayInterface.h"
#include "Character/SPlayerState.h"
#include "Core/SSaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void USSaveGameSubsystem::SaveGame()
{
	AGameStateBase* GameState = GetGameInstance()->GetWorld()->GetGameState();
	RETURN_IF_NULL_ENSURE(GameState);
	
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
void USSaveGameSubsystem::LoadGame(FString SelectedSaveSlot)
{
	if (SelectedSaveSlot.Len() > 0)
	{
		SlotName = SelectedSaveSlot;
	}
	
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

void USSaveGameSubsystem::HandleStartingNewPlayer(APlayerController* NewPlayer)
{
	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	RETURN_IF_NULL_ENSURE(PlayerState);

	PlayerState->LoadPlayerState(CurrentSavedGame);
}
