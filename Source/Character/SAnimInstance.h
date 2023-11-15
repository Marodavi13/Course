

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

class USActionComponent;
/**
 * 
 */
UCLASS()
class COURSE_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, Category = "Animation")
	bool bIsStunned = false;

	UPROPERTY(Transient, SkipSerialization, BlueprintReadOnly, Category = "Animation")
	USActionComponent* ActionComponent;

};
