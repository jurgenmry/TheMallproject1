// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/Interactables/WeaponInteractableActor.h"

//System includes
#include "MallProject/Components/WeaponComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

//Custome includes

AWeaponInteractableActor::AWeaponInteractableActor()
{
	WeaponComps = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComps"));
}

void AWeaponInteractableActor::Interact(AMallProjectCharacter* CharacterReference)
{
	Super::Interact(CharacterReference);

	//GetBoxComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//GetSphereComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	//FString InteractWeapon = "InteractWithWeapon";
	//GEngine->AddOnScreenDebugMessage(10, 5.0f, FColor::Red, InteractWeapon, 1);



	//WeaponComps->AttachWeapon(CharacterReference);
}
