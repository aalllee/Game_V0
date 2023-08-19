// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAME_V0_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void EnteredInteractionZone(AActor* InteractableActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void LeftInteractionZone(AActor* InteractableActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|ToInteract")
	void InteractRequest(AActor* InteractableActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|ToInteract")
	void SetInteractableItem(class AItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|ToInteract")
	void SphereTraceInteractableItems();

	


	
};
