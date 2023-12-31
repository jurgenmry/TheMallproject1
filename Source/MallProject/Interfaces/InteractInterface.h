// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"



UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisplayName = "Pickup"),
	Weapon UMETA(DisplayName = "Weapon"), //Technically weapon is pickup
	NonPlayableCharacter UMETA(DisplayName = "NonPlayableCharacter"),
	Device UMETA(DisplayName = "Device"), //Windows, doors, etc
	Toggle UMETA(DisplayName = "Toggle"), //Buttons, Switch, Leaver, Use to active devices
	Container UMETA(DisplayName = "Container")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

		FInteractableData()
		: InteractableType(EInteractableType::Pickup)
		, Name(FText::GetEmpty())
		, Action(FText::GetEmpty())
		, Quantity(0)
		, InteractionDuration(0.0f)
	{

	};

	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	//Only for Pickups
	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	//Use for doors, valves, that requier interaction timer.
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
};



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MALLPROJECT_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void BeginFocus() = 0;

	virtual void EndFocus() = 0;

	virtual void BeginInteract() = 0;

	virtual void EndInteract() = 0;

	virtual void Interact(class AMallProjectCharacter* CharacterReference) = 0;

	FInteractableData InteractableData;

};
