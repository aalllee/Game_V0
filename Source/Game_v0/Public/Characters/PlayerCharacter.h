// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/InteractInterface.h"
#include "Characters/PlayerCharacterState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;

class AItem;
class UNiagaraSystem;
class UNiagaraComponent;


USTRUCT(BlueprintType)
struct FCameraState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraBoomLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector CameraBoomSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float FOV;

	FCameraState() : CameraBoomLength(400.f), CameraBoomSocket(FVector(0, 0, 50)), FOV(90.f) {}
	FCameraState(float BoomLength, FVector Socket, float FOV) : CameraBoomLength(BoomLength), CameraBoomSocket(Socket), FOV(FOV) {}
};



UCLASS()
class GAME_V0_API APlayerCharacter : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
 
/// ITEM INTERACTABLE INTERFACE
public:	
	virtual void SetInteractableItem_Implementation(AItem* Item) override;
	virtual void SphereTraceInteractableItems_Implementation() override; 

protected:
	UPROPERTY(BlueprintReadWrite)
	FVector ThrowVelocity = FVector(0.f, 0.f, 0.f);

	UPROPERTY(BlueprintReadOnly)
	AItem* InteractableItem;
private:
	
	

	UPROPERTY()
	bool bDoItemTrace = true;

	UPROPERTY()
	FName ItemSocketName = FName("ItemHold");


//INPUT ACTIONS //////////////////////////////////////////////////////////////////
public:
	UPROPERTY(BlueprintReadWrite, Category = "Input")
		bool bIgnoreAllInputActions = false;

	UPROPERTY(BlueprintReadWrite, Category = "Input")
		bool bIgnoreInteractionInput = false;
protected: 
		
		UPROPERTY(EditAnywhere, Category = Input)
		UInputMappingContext* CharacterContext;

		UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* MovementAction;

		UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* LookAction;

		UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* JumpAction;

		UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* InteractAction;

		UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* AimAction;

		UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* ChargeThrowAction;


		UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* LiftUpAction;

		UPROPERTY()
		EActionState PlayerActionState = EActionState::EAS_Unoccupied;
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void ChargeThrow(const FInputActionValue& Value);
	void ChargeThrowStart(const FInputActionValue& Value);
	void ChargeThrowEnd(const FInputActionValue& Value);

	UPROPERTY()
	bool JumpStart = false;


//CAMERA//////////////////////////////////////////////////////////////////
	protected:
		UFUNCTION(BlueprintImplementableEvent)
		void CameraTransition(FCameraState NewCameraState);

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FCameraState MainCameraState {400.f, FVector(0.f, 0.f, 50.f), 90.f};
									
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FCameraState HoldingCameraState = { 350.f, FVector(0.f,100.f,50.f),90.f };

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FCameraState ChargingThrowCameraState = { 200.f, FVector(0.f,100.f,50.f),45.f };
	
		UPROPERTY(BlueprintReadWrite, Category = "Camera")
		FCameraState CurrentCameraState = MainCameraState;

		UPROPERTY(BlueprintReadWrite, Category = "Camera")
		FVector CurrentCameraBoomSocket = FVector(0.f, 0.f, 50.f);

		UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
		USpringArmComponent* CameraBoom;

		UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
		UCameraComponent* ViewCamera;

//UTILITY//////////////////////////////////////////////////////////////////
	protected:
		UFUNCTION(BlueprintCallable)
			void FindNearbyItem();

		UPROPERTY(EditAnywhere, Category = "Trace")
			float TraceRadius = 100.f;

		UPROPERTY(EditAnywhere, Category = "Trace")
			float TraceDistance = 2000.f;

		UPROPERTY(EditAnywhere, Category = "Trace")
			float TraceFrequency = 0.5f;

		UPROPERTY(EditAnywhere, Category = "Trace")
			bool drawTraceDebug = true;

		FTimerHandle TimerHandle;
//ACCESSORS//////////////////////////////////////////////////////////////////
	public:
		FORCEINLINE bool GetJumpStart() { return JumpStart; }
		FORCEINLINE void SetJumpStart(bool newJumpStart) { this->JumpStart = newJumpStart; }
		FORCEINLINE void SetDoItemTrace(bool ShouldTraceItems) { this->bDoItemTrace = ShouldTraceItems; }
		
		UFUNCTION(BlueprintCallable)
		void SetActionState(EActionState NewActionState);

		UFUNCTION(BlueprintCallable)
		FORCEINLINE FName GetItemSocketName() { return ItemSocketName; }
		UFUNCTION(BlueprintCallable)
		FORCEINLINE EActionState GetActionState() { return PlayerActionState; }

//VFX//////////////////////////////////////////////////////////////////
	public:
		UFUNCTION(BlueprintImplementableEvent, Category = "Throw")
		void DisplayThrowPath();

		UFUNCTION(BlueprintImplementableEvent, Category = "Throw")
		void SpawnProjectilePathNiagaraSystem();

		UFUNCTION(BlueprintImplementableEvent, Category = "Throw")
		void DestroyProjectilePathNiagaraSystem();


		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "VFX")
			UNiagaraSystem* NS_ProjectilePath;

		UPROPERTY(BlueprintReadWrite, Category = "VFX")
			UNiagaraComponent* NC_ProjectilePath;
};
