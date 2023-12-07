// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MallProject/Interactables/InteractableActor.h"
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
	

private:


protected:

	//================================================================================//
	// FUNCTIONS
	//================================================================================//



};
