// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mallproject/Interactables/InteractableActor.h"
#include "CCTVCamera.generated.h"

/**
 * 
 */
UCLASS()
class MALLPROJECT_API ACCTVCamera : public AInteractableActor
{
	GENERATED_BODY()

public:

	//UPROPERTY(EditAnywhere, Category = "Cameras")
	//TSubclassOf<class CCTVWidget>CameraWidget; 

	UPROPERTY(EditAnywhere, Category = "Cameras")
	TArray<TSubclassOf<class UCameraComponent>> CamerasActor;

	virtual void Interact(class AMallProjectCharacter* CharacterReference) override;
	
};
