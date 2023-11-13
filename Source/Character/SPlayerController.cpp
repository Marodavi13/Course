


#include "Character/SPlayerController.h"

void ASPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BPI_BeginPlayingState();
}
