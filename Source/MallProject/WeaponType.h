#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	Melee UMETA(DisplayName = "Melee"),
	SuperLamp UMETA(DisplayName = "SuperLamp"),
	Other UMETA(DisplayName = "Other")
};