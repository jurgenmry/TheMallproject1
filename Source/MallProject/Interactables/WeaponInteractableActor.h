// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MallProject/Interactables/InteractableActor.h"
#include "WeaponInteractableActor.generated.h"

/**
 * 
 */
UENUM()
enum class EWeaponType : uint8
{
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"), //Technically weapon is pickup
	Shotgun UMETA(DisplayName = "Shotgun"),
	Melee UMETA(DisplayName = "Melee"), //Windows, doors, etc
	SuperLamp UMETA(DisplayName = "SuperLamp"), //Buttons, Switch, Leaver, Use to active devices
	Other UMETA(DisplayName = "Other")
};

UCLASS()
class MALLPROJECT_API AWeaponInteractableActor : public AInteractableActor
{
	GENERATED_BODY()

public:

	//================================================================================//
	// Variables & Properties
	//================================================================================//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UWeaponComponent* WeaponComps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	EWeaponType WeaponType;

	//================================================================================//
	// FUNCTIONS
	//================================================================================//

	AWeaponInteractableActor();

	virtual void Interact(class AMallProjectCharacter* CharacterReference) override;

	virtual void BeginInteract() override;

	virtual void EndInteract() override;
};
