#include "Actor/PickUpActor/SPickUpCoin.h"

#include "Course.h"
#include "Character/SPlayerState.h"

ASPickUpCoin::ASPickUpCoin()
{
}

void ASPickUpCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	ASPlayerState* PlayerState = Cast<ASPlayerState>(InstigatorPawn->Controller->PlayerState);
	RETURN_IF_NULL(PlayerState);

	PlayerState->AddCredits(CreditsOnInteract, TEXT("Coin Interaction"));
	Destroy();
}

void ASPickUpCoin::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APawn* OtherPawn = Cast<APawn>(OtherActor);
	RETURN_IF_NULL(OtherPawn);

	Execute_Interact(this, OtherPawn);
}
