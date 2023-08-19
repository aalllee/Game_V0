// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/ItemPickup.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = CurrentCameraState.CameraBoomLength;
	CameraBoom->SocketOffset = CurrentCameraState.CameraBoomSocket;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterContext, 0);
		}
	}

	//GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::FindNearbyItem, TraceFrequency, true, 0.5f);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::FindNearbyItem, TraceFrequency, true, 0.5f);
}

void APlayerCharacter::FindNearbyItem()
{
	if(bDoItemTrace) Execute_SphereTraceInteractableItems(this);
}
	
	
	


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Cast player input to EnhancedInputComponent and Bind Action
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
		EnhancedInputComponent->BindAction(ChargeThrowAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ChargeThrow);
		EnhancedInputComponent->BindAction(ChargeThrowAction, ETriggerEvent::Started, this, &APlayerCharacter::ChargeThrowStart);
		EnhancedInputComponent->BindAction(ChargeThrowAction, ETriggerEvent::Completed, this, &APlayerCharacter::ChargeThrowEnd);
	}

}
void APlayerCharacter::SetActionState(EActionState NewActionState)
{
	//this->PlayerActionState = NewActionState;

	if (PlayerActionState == NewActionState)
		return;

	// Handle state-specific actions
	switch (NewActionState)
	{
	case EActionState::EAS_Unoccupied:
		// Implement state change logic
		this->bUseControllerRotationYaw = false;
		CameraTransition(MainCameraState);

		if (NC_ProjectilePath)
			DestroyProjectilePathNiagaraSystem();

		if (PlayerActionState == EActionState::EAS_Carrying)
			SetDoItemTrace(true);


		if (PlayerActionState == EActionState::EAS_ChargingThrow)
			SetDoItemTrace(true);

		//ExitSideView();

		break;
	case EActionState::EAS_Carrying:
		// Implement state change logic
		this->bUseControllerRotationYaw = false;
		CameraTransition(HoldingCameraState);

		if (NC_ProjectilePath)
			DestroyProjectilePathNiagaraSystem();
		
		if (PlayerActionState == EActionState::EAS_Unoccupied)
			SetDoItemTrace(false);

		
		//EnterSideView();

		break;
	case EActionState::EAS_ChargingThrow:
		
		this->bUseControllerRotationYaw = true;
		CameraTransition(ChargingThrowCameraState);
		// Implement state change logic
		SpawnProjectilePathNiagaraSystem();
	
		break;
	default:
		break;
	}

	PlayerActionState = NewActionState;

}
void APlayerCharacter::SetInteractableItem_Implementation(AItem* Item)
{
	if (Item == InteractableItem) return;
	

	
	//remove previuos selection's outline
	if (InteractableItem)
		InteractableItem->RenderCustomDepthPass(false);

	
	this->InteractableItem = Item;

	if (InteractableItem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString("ITEM SET TO")+InteractableItem->GetName());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString("ITEM SET TO NULL"));
	}
	
	if (Item)
		Item->SetInteractingCharacter(this);

	//set current selection's outline
	if (InteractableItem)
		InteractableItem->RenderCustomDepthPass(true);

}

void APlayerCharacter::SphereTraceInteractableItems_Implementation()
{
	const FVector Start = ViewCamera->GetComponentLocation();
	const FVector End = Start + ViewCamera->GetForwardVector() * TraceDistance;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	TArray<FHitResult> HitArray;
	EDrawDebugTrace::Type typeDebug = EDrawDebugTrace::Type::ForDuration;
	FHitResult OutHit;

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	const bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		Start,
		End,
		TraceRadius,
		objectTypesArray,
		false,
		ActorsToIgnore,
		drawTraceDebug ? EDrawDebugTrace::Type::ForDuration : EDrawDebugTrace::Type::None,
		OutHit,
		true,
		FLinearColor::Blue,
		FColor::Green,
		TraceFrequency
	);

	AItem* Item = Cast<AItem>(OutHit.GetActor());
	if (Hit && Item)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString("SET") + GetDebugName(OutHit.GetActor()));
		Execute_SetInteractableItem(this, Item);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString("SET ITEM NULL"));

		Execute_SetInteractableItem(this, nullptr);
	}
}





void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (bIgnoreAllInputActions) return;

	GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Orange, FString("moveCalled"));

	//type based on the mapping setup in unreal
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);

}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Orange, FString::SanitizeFloat(LookAxisVector.X));
	GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Cyan, FString::SanitizeFloat(LookAxisVector.Y));
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);

}

void APlayerCharacter::Jump(const FInputActionValue& Value)
{
	if (bIgnoreAllInputActions) return;

	//jump
	//GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Orange, FString("jump"));
	JumpStart = true;
	GetCharacterMovement()->DoJump(true);
	//GetCharacterMovement()->DoJump(true);
}

void APlayerCharacter::Interact(const FInputActionValue& Value)
{
	if (bIgnoreAllInputActions || bIgnoreInteractionInput) return;

	if (InteractableItem)
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Orange, FString("Player Interact Called"));

		//bDoItemTrace = false;
		InteractableItem->Interact();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Orange, FString("ItemNULL"));
	}
}



void APlayerCharacter::ChargeThrow(const FInputActionValue& Value)
{
	if (bIgnoreAllInputActions) return;

	if (PlayerActionState == EActionState::EAS_ChargingThrow)
	{
		GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Yellow, FString("Charging"));
		DisplayThrowPath();
	}
}

void APlayerCharacter::ChargeThrowStart(const FInputActionValue& Value)
{
	if (bIgnoreAllInputActions) return;

	if (InteractableItem
		&& InteractableItem->GetItemState() == EItemState::EIS_PickedUp
		&& PlayerActionState == EActionState::EAS_Carrying)
	{
		GEngine->AddOnScreenDebugMessage(2, 15.f, FColor::Green, FString("ChargeThrowSTARTED"));
		DisplayThrowPath();
		SetActionState(EActionState::EAS_ChargingThrow);
	}
}

void APlayerCharacter::ChargeThrowEnd(const FInputActionValue& Value)
{
	if (bIgnoreAllInputActions) return;

	if (AItemPickup* ItemPickup = Cast<AItemPickup>(InteractableItem))
	{
		if (PlayerActionState == EActionState::EAS_ChargingThrow)
		{
			ItemPickup->BP_Throw(ThrowVelocity);
			SetActionState(EActionState::EAS_Unoccupied);
		}
		
	}
		
}


