// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "ItemPickup.generated.h"

/**
 * 
 */
UCLASS()
class GAME_V0_API AItemPickup : public AItem
{
	GENERATED_BODY()
	
public:
	virtual void Interact() override;
	void Throw();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Pickup(FName ItemSocketName);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlaceDown();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Throw(FVector Velocity);

protected:
	UPROPERTY(BlueprintReadWrite)
	float ThrownToIdleTransitionTime = 0.1f;

};
