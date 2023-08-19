// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacterAnimInstance.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (PlayerCharacter)
	{
		PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
	}
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if(PlayerCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);
		IsFalling = PlayerCharacterMovement->IsFalling();
		JumpStart = PlayerCharacter->GetJumpStart();
		PlayerActionState = PlayerCharacter->GetActionState();
		UpdateLeanAmount(DeltaTime);
		
	}
}

void UPlayerCharacterAnimInstance::JumpStartOff()
{
	PlayerCharacter->SetJumpStart(false);
}

FVector UPlayerCharacterAnimInstance::UpdateLeanAmount(float DeltaTime)
{
	FVector acceleration;
	if (PlayerCharacterMovement && PlayerCharacter)
	{
		FVector CurrentVelocity = PlayerCharacterMovement->Velocity;
		acceleration = (CurrentVelocity - PreviousVelocity)/DeltaTime;
		PreviousVelocity = CurrentVelocity;

		float dot  = FVector::DotProduct(CurrentVelocity, acceleration);
		FRotator PlayerRotation = PlayerCharacter->GetActorRotation();

		float maxAcceleration;
		if (dot > 0.f)
			maxAcceleration = PlayerCharacterMovement->GetMaxAcceleration();
		else
			maxAcceleration = PlayerCharacterMovement->GetMaxBrakingDeceleration();

		acceleration.GetClampedToMaxSize(maxAcceleration);
		acceleration /= maxAcceleration;

		RelativeAcceleration = UKismetMathLibrary::Quat_UnrotateVector(PlayerRotation.Quaternion(), acceleration);

		LeanLR = UKismetMathLibrary::FInterpTo(LeanLR, RelativeAcceleration.Y, DeltaTime, LeanInterpolationSpeed);

	
	}
	
	return acceleration;
}
