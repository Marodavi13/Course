

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COURSE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName="BeginPlayingState")
	void BPI_BeginPlayingState();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(Transient, SkipSerialization)
	UUserWidget* PauseMenu;

	UFUNCTION(BlueprintCallable, Category="")
	void TogglePause();

	virtual void SetupInputComponent() override;
};
