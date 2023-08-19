// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemPickup.h"
#include "Characters/PlayerCharacter.h"



void AItemPickup::Interact()
{
    // Handle state-specific actions
    switch (CurrentItemState)
    {
    case EItemState::EIS_Idle:
        BP_Pickup(PlayerCharacter->GetItemSocketName());
        PlayerCharacter->SetActionState(EActionState::EAS_Carrying);
        return;

        break;
    case EItemState::EIS_PickedUp:
        BP_PlaceDown();
        return;

        break;
    case EItemState::EIS_Thrown:
        // Implement Throw state logic



        break;
    default:
        break;
    }
}

void AItemPickup::Throw()
{



}
