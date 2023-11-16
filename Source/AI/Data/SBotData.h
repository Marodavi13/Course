

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SBotData.generated.h"

class USActionBase;
/**
 * 
 */
UCLASS()
class COURSE_API USBotData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	TSubclassOf<AActor> BotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	TArray<TSubclassOf<USActionBase>> Actions;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	UTexture2D* Icon;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Bots"), GetFName());
	}
};
