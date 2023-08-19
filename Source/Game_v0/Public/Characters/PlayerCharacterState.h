#pragma once
#pragma once


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_CanInteract UMETA(DisplayName = "Can Interact"),
	ECS_Interacting UMETA(DisplayName = "Interacting")

};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Carrying UMETA(DisplayName = "Carrying"),
	EAS_ChargingThrow UMETA(DisplayName = "ChargingThrow"),
	EAS_Aiming UMETA(DisplayName = "Aiming"),
	EAS_Throwing UMETA(DisplayName = "Throwing"),
	EAS_Transitional UMETA(DisplayName = "Transitional")
	
};