#pragma once
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	E9_mm UMETA(DisplayName = "9mm"),
	AR UMETA(DisplayName = "Assault Rifle"),
	Shootgun UMETA(DisplayName = "Shotgun"),
	LightBatteries UMETA(DisplayName = "Bateries"),
	Other UMETA(DisplayName = "Other Ammo") // This is for doors and others
};