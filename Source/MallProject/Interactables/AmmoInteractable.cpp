// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/Interactables/AmmoInteractable.h"

//System include


//custome include


AAmmoInteractable::AAmmoInteractable()
{
	InstaceInteractableData.InteractableType = EInteractableType::Pickup; //This should be Ammo
	ItemState = EItemState::Ready_For_Pickup;
}

void AAmmoInteractable::BeginPlay()
{
}

void AAmmoInteractable::Tick(float DeltaTime)
{
}
