// Fill out your copyright notice in the Description page of Project Settings.


#include "MallProject/Interactables/WeaponInteractableActor.h"

//System includes
#include "MallProject/Components/WeaponComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

//Custome includes

AWeaponInteractableActor::AWeaponInteractableActor()
	: AmmoCount(0)
	, WeaponType(EWeaponType::Pistol)
	, AmmoType(EAmmoType::E9_mm)
	, MagazineCapacity(10)
	, ReloadMontageSection(FName(TEXT("Reload_Pistol")))
{
	//WeaponComps = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComps"));
	WeaponType = EWeaponType::Pistol;
	
	//ItemState = EItemState::ReadyForPickup;
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

void AWeaponInteractableActor::BeginInteract()
{
	//Play character animation
	switch (WeaponType)
	{
	case EWeaponType::Pistol:
		break;
	case EWeaponType::Rifle:
		break;
	case EWeaponType::Shotgun:
		break;
	case EWeaponType::Melee:
		break;
	case EWeaponType::SuperLamp:
		break;
	case EWeaponType::Other:
		break;
	default:
		break;
	}
}

void AWeaponInteractableActor::EndInteract()
{
	//
}

void AWeaponInteractableActor::DecrementAmmo()
{
	if (AmmoCount - 1 <= 0 )
	{
		AmmoCount = 0;
	}
	else
	{
		--AmmoCount;
	}
}

void AWeaponInteractableActor::ReloadAmmo(int32 Ammount)
{

	AmmoCount += Ammount;
}
