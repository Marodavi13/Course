// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "SWorldUserWidget.generated.h"
class USizeBox;
/**
 * 
 */
UCLASS()
class COURSE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachedActor;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="UI")
	USizeBox* ParentSizeBox;

	UPROPERTY(EditAnywhere, Category="UI")
	FVector WorldOffset = FVector::UpVector * 90.f;

};
