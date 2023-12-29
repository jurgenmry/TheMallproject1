// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MallProject/Interactables/InteractableActor.h"
#include "MallProject/AmmoType.h"
#include "AmmoInteractable.generated.h"

/**
 * 
 */
UCLASS()
class MALLPROJECT_API AAmmoInteractable : public AInteractableActor
{
	GENERATED_BODY()

public: 

	//================================================================================//
	// FUNCTIONS
	//================================================================================//


	AAmmoInteractable();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Interact(class AMallProjectCharacter* CharacterReference) override;

	/**** Getters ****/
	
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE int32 GetItemCount() const { return ItemCount; }

	/**** Setters ****/

private:

	//================================================================================//
	// Variables
	//================================================================================//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Ammo", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

protected:

	//================================================================================//
	// FUNCTIONS
	//================================================================================//



};
