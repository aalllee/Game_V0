// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


//#include "InteractInterface.h"
// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(ItemMesh);

	ItemWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemWidgetComponent"));
	ItemWidgetComponent->SetupAttachment(ItemMesh);
}

void AItem::SetInteractingCharacter(APlayerCharacter* Character)
{
	this->PlayerCharacter = Character;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	ItemWidgetComponent->SetVisibility(false);

	PlayerCharacter = Cast<APlayerCharacter> (UGameplayStatics::GetPlayerCharacter(this,0));
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlapperComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString("Overlaped")+OtherActor->GetName());
	

	IInteractInterface* InteractInterface = Cast<IInteractInterface>(OtherActor);
	
	if (InteractInterface)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	//	if (PlayerCharacter->InteractableItem->isActive) return;


		if (ItemWidgetComponent)
		{
			ItemWidgetComponent->SetVisibility(true);
		}
		
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Orange, FString("Overlaped and Implements"));
		//InteractInterface->SetInteractableItem(this);
		InteractInterface->Execute_SetInteractableItem(OtherActor,this);
		
	}
	
	
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	
	IInteractInterface* InteractInterface = Cast<IInteractInterface>(OtherActor);
	if (InteractInterface)
	{
		
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Orange, FString("Overlaped and Implements"));
		//InteractInterface->SetInteractableItem(this);
		InteractInterface->Execute_SetInteractableItem(OtherActor, nullptr);


		if (ItemWidgetComponent)
		{
			ItemWidgetComponent->SetVisibility(false);
		}
	}
}

void AItem::Interact()
{
		BP_Interact();
}

void AItem::RenderCustomDepthPass(bool value)
{
	ItemMesh->SetRenderCustomDepth(value);
}
