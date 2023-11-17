

#pragma once

#include "CoreMinimal.h"
#include "Core/SSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SSaveGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class COURSE_API USSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
		
	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void SaveGame();
	
	virtual void LoadGame(FString SelectedSaveSlot);

	virtual void HandleStartingNewPlayer(APlayerController* NewPlayer);

protected:
	
	UPROPERTY()
	FString SlotName = TEXT("SaveGame");

	UPROPERTY()
	TObjectPtr<USSaveGame> CurrentSavedGame = nullptr;


};
