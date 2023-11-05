// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MallProject/Interactables/InteractableActor.h"
#include "WeaponInteractableActor.generated.h"

/**
 * 
 */
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


	//================================================================================//
	// FUNCTIONS
	//================================================================================//

	AWeaponInteractableActor();

	virtual void Interact(class AMallProjectCharacter* CharacterReference) override;
};
