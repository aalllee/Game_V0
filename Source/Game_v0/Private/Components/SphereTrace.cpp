// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SphereTrace.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values for this component's properties
USphereTrace::USphereTrace()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

const bool USphereTrace::SphereTrace()
{



	const FVector Start = GetOwner()->GetActorLocation();
	const FVector End = Start+GetOwner()->GetActorForwardVector()* TraceDistance;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	TArray<FHitResult> HitArray;
	EDrawDebugTrace::Type typeDebug = EDrawDebugTrace::Type::ForDuration;
	TArray<FHitResult> OutHit;
	const bool Hit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		Start,
		End,
		TraceRadius,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::Type::ForDuration,
		OutHit,
		true,
		FLinearColor::Blue,
		FColor::Green,
		60.f
	);




	return Hit;
}


// Called when the game starts
void USphereTrace::BeginPlay()
{
	Super::BeginPlay();
	
	

}


// Called every frame
void USphereTrace::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

