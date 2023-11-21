// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MallProject/Interactables/InteractableActor.h"
#include "Components/TimelineComponent.h"
#include "DoorInteractable.generated.h"

/**
 * 
 */

UCLASS()
class MALLPROJECT_API ADoorInteractable : public AInteractableActor
{
	GENERATED_BODY()

public:

	bool bOpen;
	bool ReadyState;
	float RotateValue;
	float CurveFloatValue;
	float TimelineValue;
	FRotator DoorRotation;
	FTimeline MyTimeLine;
	
	UPROPERTY(EditAnywhere)
	class UCurveFloat* DoorCurve;


	ADoorInteractable();

	UFUNCTION()
	void RotateDoor();

	UFUNCTION()
	void SetDoorState();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginFocus() override;

	virtual void EndFocus() override;

	virtual void BeginInteract() override;

	virtual void EndInteract() override;

	virtual void Interact(class AMallProjectCharacter* CharacterReference) override;
};
