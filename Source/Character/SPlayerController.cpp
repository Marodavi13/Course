


#include "Character/SPlayerController.h"

#include "Course.h"
#include "Blueprint/UserWidget.h"

void ASPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BPI_BeginPlayingState();
}

void ASPlayerController::TogglePause()
{
	if(PauseMenu && PauseMenu->IsInViewport())
	{
		PauseMenu->RemoveFromParent();
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		
		PauseMenu = CreateWidget<UUserWidget>(this, PauseMenuClass);
		RETURN_IF_NULL_ENSURE(PauseMenu);

		PauseMenu->AddToViewport(999);
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
	}
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("PauseMenu"), IE_Pressed, this, &ASPlayerController::TogglePause);
}
