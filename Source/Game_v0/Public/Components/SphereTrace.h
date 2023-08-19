// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SphereTrace.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_V0_API USphereTrace : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USphereTrace();

	UFUNCTION(BlueprintCallable)
	const bool SphereTrace();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceRadius = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance = 2000.f;
};
