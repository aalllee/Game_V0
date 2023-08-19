// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"

#include "Item.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;
class APlayerCharacter;




UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Idle UMETA(DisplayName = "Idle"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Thrown UMETA(DisplayName = "Thrown")
	
};


UCLASS()
class GAME_V0_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlapperComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	
	UFUNCTION(Category = "Item")
	virtual void Interact();

	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void BP_Interact();

	UFUNCTION()
	void SetInteractingCharacter(APlayerCharacter* Character);

	UFUNCTION()
		FORCEINLINE EItemState GetItemState() { return CurrentItemState; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	bool isActive = false;

	UPROPERTY(BlueprintReadWrite)
	bool CanInteract = true;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	EItemState CurrentItemState = EItemState::EIS_Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;
private:
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* ItemWidgetComponent;
	
	

public:
	void RenderCustomDepthPass(bool value);
};
