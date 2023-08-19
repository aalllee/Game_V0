// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/PlayerCharacterState.h"
#include "PlayerCharacterAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class GAME_V0_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool JumpStart;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool JumpToHoverStart = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float LeanLR = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float LeanInterpolationSpeed = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	FVector RelativeAcceleration;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	EActionState PlayerActionState = EActionState::EAS_Unoccupied;

protected:
	UFUNCTION(BlueprintCallable)
	void JumpStartOff();

private:
	FVector PreviousVelocity = FVector(0.f, 0.f, 0.f);

	UFUNCTION()
		FVector UpdateLeanAmount(float DeltaTime);
};
