// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/Interactables/AmmoInteractable.h"

//System include

//custome include
#include "MallProject/MallProjectCharacter.h"

AAmmoInteractable::AAmmoInteractable()
{
	InstaceInteractableData.InteractableType = EInteractableType::Pickup; //This should be Ammo
	ItemState = EItemState::Ready_For_Pickup;
}

void AAmmoInteractable::BeginPlay()
{
	Super::BeginPlay();
}

void AAmmoInteractable::Tick(float DeltaTime)
{
}

void AAmmoInteractable::Interact(AMallProjectCharacter* CharacterReference)
{
	Super::Interact(CharacterReference);

	if (CharacterReference->AmmoMap.Find(this->GetAmmoType()))
	{
		//Get the amount of ammo in our map for ammos type
		int32 AmmoCount = { CharacterReference->AmmoMap[this->GetAmmoType()] };
		AmmoCount += this->GetItemCount();

		//Set the amount of ammo in the map for this type
		CharacterReference->AmmoMap[this->GetAmmoType()] = AmmoCount;

		Destroy();
	}
	
}

